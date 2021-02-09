#include "logManager.h"

#include "readini.h"

#include <QTimer>

pid_t gettid()
{
#ifdef WIN32
    return 0;
#else
    return syscall(__NR_gettid);
#endif
}

#ifndef WIN32
pthread_once_t LogManager::m_pthOnce = PTHREAD_ONCE_INIT;
#else
std::once_flag LogManager::m_pthOnceFlag;
#endif

LogManager* LogManager::m_instance;


LogManager::LogManager(QObject *parent) : QObject(parent)
{
    ReadIni readini;
    readini.setIniName("./logSystem.ini");
    QString qsLogLevel = readini.getValue("Log", "logLevel", "");
    m_eLogLevel = (qsLogLevel.isEmpty() ? LOG_INFO : qsLogLevel.toInt());

    LogBuffer *head = new LogBuffer(m_nSingleBufLen);
    if (head == nullptr) {
        std::ofstream off("log.txt", std::ios::app);
        off << "no space to allocate buffer\n" << std::endl;
        off.close();
        exit(1);
    }

    LogBuffer *curBuffer, *preBuffer = head;
    for (int i = 1; i < m_nBufferCnt; i++) {
        curBuffer = new LogBuffer(m_nSingleBufLen);
        if (curBuffer == nullptr) {
            std::ofstream off("log.txt", std::ios::app);
            off << "no space to allocate buffer\n" << std::endl;
            off.close();
            exit(1);
        }
        curBuffer->prev = preBuffer;
        preBuffer->next = curBuffer;
        preBuffer = preBuffer->next;
    }
    preBuffer->next = head;
    head->prev = preBuffer;

    pCurBuffer = head;
    pToWriteBuffer = head;
    m_pid = getpid();
}

LogManager::~LogManager()
{

}

void LogManager::initThread()
{
    if (m_instance == nullptr) {
        m_instance = new LogManager();
    }
}

void LogManager::init(const char* pLogPath, const char* pLogName, int nLevel)
{
#ifndef WIN32
    pthread_mutex_lock(&m_mutex);
#else
    std::unique_lock<std::mutex> locker(m_mutex);
#endif

    strncpy(m_pLogPath, pLogPath, 256);
    strncpy(m_pLogName, pLogName, 128);

    QDir qdir;
    qdir.mkpath(m_pLogPath);

    if (access(m_pLogPath, F_OK | W_OK) == -1)
    {
        std::ofstream off("log.txt", std::ios::app);
        off << "logdir error: " << std::endl;
        off.close();
    }
    else
    {
        m_bInited = true;
    }
    m_eLogLevel = (nLevel >= LOG_NOLEVEL ? LOG_FATAL : nLevel);
    m_eLogLevel = (nLevel <= LOG_INFO ? LOG_INFO : nLevel);

#ifndef WIN32
    pthread_mutex_unlock(&m_mutex);
#else
    locker.unlock();
#endif
}

int LogManager::getLevel()
{
    return m_eLogLevel;
}

void LogManager::runThread()
{
    while (true) {
#ifndef WIN32
        pthread_mutex_lock(&m_mutex);
#else
        std::unique_lock<std::mutex> locker(m_mutex);
#endif

        if (pToWriteBuffer->m_nType == LogBuffer::Buffer_Valiable)
        {
            struct timespec tsp;
            struct timeval now;
#ifndef WIN32
            gettimeofday(&now, NULL);
            tsp.tv_sec = now.tv_sec;
            tsp.tv_nsec = now.tv_usec * 1000;
#else
            QTime qTime = QTime::currentTime();
            tsp.tv_sec = qTime.second();
            tsp.tv_nsec = qTime.msec() * 1000 * 1000;
#endif
            tsp.tv_sec += m_nBufferWaitTime;
#ifndef WIN32
            pthread_cond_timedwait(&m_cond, &m_mutex, &tsp);
#else
            auto cNow = std::chrono::system_clock::now();
            m_cond.wait_until(locker, cNow + std::chrono::seconds(m_nBufferWaitTime));
#endif
        }

        if (pToWriteBuffer->isEmpty())
        {
#ifndef WIN32
            pthread_mutex_unlock(&m_mutex);
#else
            locker.unlock();
#endif
            continue;
        }

        if (pToWriteBuffer->m_nType == LogBuffer::Buffer_Valiable)
        {
            assert(pCurBuffer == pToWriteBuffer);
            pCurBuffer->m_nType = LogBuffer::Buffer_FULL;
            pCurBuffer = pCurBuffer->next;
        }

        int year = m_stTimer.year, mon = m_stTimer.mon, day = m_stTimer.day;
#ifndef WIN32
        pthread_mutex_unlock(&m_mutex);
#else
        locker.unlock();
#endif

        if (!checkLogFile(year, mon, day)) {
            continue;
        }
        pToWriteBuffer->writeToFile(m_pFile);
        fflush(m_pFile);

#ifndef WIN32
        pthread_mutex_lock(&m_mutex);
#else
        locker.lock();
#endif
        pToWriteBuffer->clear();
        pToWriteBuffer = pToWriteBuffer->next;

#ifndef WIN32
        pthread_mutex_unlock(&m_mutex);
#else
        locker.unlock();
#endif
    }
}

void LogManager::addLog(const char* pLevel, const char* format, ...)
{
    int ms;
    uint64_t curr_sec = m_stTimer.get_curr_time(&ms);
    if (m_nLastTime && curr_sec - m_nLastTime < m_nRelogThresold)
        return ;

    char log_line[m_nLogLineSize];
    int prev_len = snprintf(log_line, m_nLogLineSize, "%s[%s.%03d]", pLevel, m_stTimer.utc_fmt, ms);

    va_list arg_ptr;
    va_start(arg_ptr, format);

    int main_len = vsnprintf(log_line + prev_len, m_nLogLineSize - prev_len, format, arg_ptr);

    va_end(arg_ptr);

    uint32_t len = prev_len + main_len;

    m_nLastTime = 0;
    bool tell_back = false;

#ifndef WIN32
    pthread_mutex_lock(&m_mutex);
#else
    std::unique_lock<std::mutex> locker(m_mutex);
#endif

    if (pCurBuffer->m_nType == LogBuffer::Buffer_Valiable && pCurBuffer->getFreeLen() >= len)
    {
        pCurBuffer->addLog(log_line, len);
    }
    else
    {
        if (pCurBuffer->m_nType == LogBuffer::Buffer_Valiable)
        {
            pCurBuffer->m_nType = LogBuffer::Buffer_FULL;
            LogBuffer* next_buf = pCurBuffer->next;
            tell_back = true;

            //it suggest that this buffer is under the persist job
            if (next_buf->m_nType == LogBuffer::Buffer_FULL)
            {
                //if mem use < MEM_USE_LIMIT, allocate new cell_buffer
                if (m_nSingleBufLen * (m_nBufferCnt + 1) > m_nMaxMememory)
                {
                    fprintf(stderr, "no more log space can use\n");
                    pCurBuffer = next_buf;
                    m_nLastTime = curr_sec;
                }
                else
                {
                    LogBuffer* new_buffer = new LogBuffer(m_nSingleBufLen);
                    m_nBufferCnt += 1;
                    new_buffer->prev = pCurBuffer;
                    pCurBuffer->next = new_buffer;
                    new_buffer->next = next_buf;
                    next_buf->prev = new_buffer;
                    pCurBuffer = new_buffer;
                }
            }
            else
            {
                pCurBuffer = next_buf;
            }
            if (!m_nLastTime)
                pCurBuffer->addLog(log_line, len);
        }
        else
        {
            m_nLastTime = curr_sec;
        }
    }

#ifndef WIN32
    pthread_mutex_unlock(&m_mutex);
#else
    locker.unlock();
#endif

    if (tell_back)
    {
#ifndef WIN32
        pthread_cond_signal(&m_cond);
#else
        m_cond.notify_one();
#endif
    }
}

//in case of one log file too large, seperate to several log files
bool LogManager::checkLogFile(int nYear, int nMonth, int nDay)
{
    if (!m_bInited)
    {
        if (m_pFile) {
            fclose(m_pFile);
        }
        m_pFile = fopen("/dev/null", "w");
        return m_pFile != NULL;
    }
    if (!m_pFile)
    {
        m_nYear = nYear;
        m_nMonth = nMonth;
        m_nDay = nDay;
        char log_path[1024] = {};
        sprintf(log_path, "%s/%s.%d%02d%02d.%u.log", m_pLogPath, m_pLogName, nYear, nMonth, nDay, m_pid);
        m_pFile = fopen(log_path, "w");
        if (m_pFile) {
            m_nLogCnt += 1;
        }
    }
    else if (m_nDay != nDay)
    {
        fclose(m_pFile);
        char log_path[1024] = {};
        m_nYear = nYear;
        m_nMonth = nMonth;
        m_nDay = nDay;
        sprintf(log_path, "%s/%s.%d%02d%02d.%u.log", m_pLogPath, m_pLogName, nYear, nMonth, nDay, m_pid);
        m_pFile = fopen(log_path, "w");
        if (m_pFile) {
            m_nLogCnt = 1;
        }
    }
    else if (ftell(m_pFile) >= m_nLogUseLimit)
    {
        fclose(m_pFile);
        char old_path[1024] = {};
        char new_path[1024] = {};

        for (int i = m_nLogCnt - 1;i > 0; --i)
        {
            sprintf(old_path, "%s/%s.%d%02d%02d.%u.log.%d", m_pLogPath, m_pLogName, nYear, nMonth, nDay, m_pid, i);
            sprintf(new_path, "%s/%s.%d%02d%02d.%u.log.%d", m_pLogPath, m_pLogName, nYear, nMonth, nDay, m_pid, i + 1);
            rename(old_path, new_path);
        }

        sprintf(old_path, "%s/%s.%d%02d%02d.%u.log", m_pLogPath, m_pLogName, nYear, nMonth, nDay, m_pid);
        sprintf(new_path, "%s/%s.%d%02d%02d.%u.log.1", m_pLogPath, m_pLogName, nYear, nMonth, nDay, m_pid);
        rename(old_path, new_path);
        m_pFile = fopen(old_path, "w");
        if (m_pFile) {
            m_nLogCnt += 1;
        }
    }
    return (m_pFile != nullptr);
}

