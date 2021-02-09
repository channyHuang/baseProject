#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QTime>
#include <fstream>

#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>

#ifndef WIN32
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "logbuffer.h"

enum LOG_LEVEL{
    LOG_INFO = 1,
    LOG_DEBUG,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_NOLEVEL
};

extern pid_t gettid();

struct utc_timer
{
    utc_timer()
    {
#ifndef WIN32
        struct timeval tv;
        gettimeofday(&tv, NULL);
        m_nSysSecond = tv.tv_sec;
#else
        QTime qTime = QTime::currentTime();
        m_nSysSecond = qTime.second();
#endif
        m_nSysMinute = m_nSysSecond / 60;
        setCurTime();

        reset_utc_fmt();
    }

    uint64_t get_curr_time(int* p_msec = nullptr)
    {
        struct timeval tv;
#ifndef WIN32      
        gettimeofday(&tv, NULL);
        if (p_msec) {
            *p_msec = tv.tv_usec / 1000;
        }
        if ((uint32_t)tv.tv_sec != m_nSysSecond)
        {
            sec = tv.tv_sec % 60;
            m_nSysSecond = tv.tv_sec;
#else
        QTime qTime = QTime::currentTime();
        tv.tv_sec = qTime.second();
        if (p_msec) {
            *p_msec = qTime.msec();
        }
        if (qTime.second() != m_nSysSecond)
        {
            sec = qTime.second() % 60;
            m_nSysSecond = qTime.second();
#endif
            if (m_nSysSecond / 60 != m_nSysMinute)
            {
                m_nSysMinute = m_nSysSecond / 60;
                setCurTime();

                reset_utc_fmt();
            }
            else
            {
                reset_utc_fmt_sec();
            }
        }
        return tv.tv_sec;
    }

    void setCurTime()
    {
        struct tm curm_stTimer;
        localtime_r((time_t*)&m_nSysSecond, &curm_stTimer);
        year = curm_stTimer.tm_year + 1900;
        mon  = curm_stTimer.tm_mon + 1;
        day  = curm_stTimer.tm_mday;
        hour  = curm_stTimer.tm_hour;
        min  = curm_stTimer.tm_min;
        sec  = curm_stTimer.tm_sec;
    }

    int year, mon, day, hour, min, sec;
    char utc_fmt[20];

private:
    void reset_utc_fmt()
    {
        snprintf(utc_fmt, 20, "%d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
    }

    void reset_utc_fmt_sec()
    {
        snprintf(utc_fmt + 17, 3, "%02d", sec);
    }

    uint64_t m_nSysMinute;
    uint64_t m_nSysSecond;
};

class LogManager : public QObject
{
    Q_OBJECT
public:
    static LogManager* getInstance() {
        pthread_once(&m_pthOnce, LogManager::initThread);
        return m_instance;
    }

    virtual ~LogManager();
    static void initThread();
    void init(const char* pLogPath, const char* pLogName, int nLevel = LOG_INFO);
    int getLevel();
    void addLog(const char* pLevel, const char* format, ...);
    void runThread();
    bool checkLogFile(int nYear, int nMonth, int nDay);

private:
    LogManager(QObject *parent = nullptr);

    LogBuffer *pCurBuffer, *pToWriteBuffer;
    int m_nBufferCnt = 3;
    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;
    char m_pLogPath[256];
    char m_pLogName[128];
    bool m_bInited = false;
    FILE* m_pFile;

    utc_timer m_stTimer;

    pid_t m_pid = 0;
    int m_nDay, m_nMonth, m_nYear, m_nLogCnt = 0;

    uint64_t m_nLastTime;

    int m_eLogLevel = LOG_NOLEVEL;
    //size
    uint32_t m_nMaxMememory = (1 << 30);
    uint32_t m_nSingleBufLen = 20 * (1 << 20);
    uint32_t m_nBufferWaitTime = 1;
    uint32_t m_nLogLineSize = (1 << 12);
    uint32_t m_nLogUseLimit = (1 << 30);
    uint32_t m_nRelogThresold = 5;

    static pthread_once_t m_pthOnce;
    static LogManager* m_instance;
};


#endif // LOGMANAGER_H
