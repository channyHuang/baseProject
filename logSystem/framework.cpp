#include "framework.h"

#include "logManager.h"

#define log(format, args...) \
    if (LogManager::getInstance()->getLevel() <= LOG_INFO) \
    { \
        LogManager::getInstance()->addLog("[INFO]", "[%u]%s:%d(%s): " format "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    }

#define log_info(format, args...) \
    if (LogManager::getInstance()->getLevel() <= LOG_INFO) \
    { \
        LogManager::getInstance()->addLog("[INFO]", "[%u]%s:%d(%s): " format "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    }

#define log_debug(format, args...) \
    if (LogManager::getInstance()->getLevel() <= LOG_DEBUG) \
    { \
        LogManager::getInstance()->addLog("[DEBUG]", "[%u]%s:%d(%s): " format "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    }

#define log_warning(format, args...) \
    if (LogManager::getInstance()->getLevel() <= LOG_WARNING) \
    { \
        LogManager::getInstance()->addLog("[WARNING]", "[%u]%s:%d(%s): " format "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    }

#define log_error(format, args...) \
    if (LogManager::getInstance()->getLevel() <= LOG_ERROR) \
    { \
        LogManager::getInstance()->addLog("[ERROR]", "[%u]%s:%d(%s): " format "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    }

#define log_fatal(format, args...) \
    if (LogManager::getInstance()->getLevel() <= LOG_FATAL) \
    { \
        LogManager::getInstance()->addLog("[FATAL]", "[%u]%s:%d(%s): " format "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    }

void* startThread(void* args)
{
    LogManager::getInstance()->runThread();
    return NULL;
}

void cStartThread()
{
    LogManager::getInstance()->runThread();
}

void init(const char* pLogPath, const char* pLogName, int nLevel)
{
    LogManager::getInstance()->init(pLogPath, pLogName, nLevel);
#ifndef WIN32
    pthread_t tid;
    pthread_create(&tid, NULL, startThread, NULL);
    pthread_detach(tid);
#else
    std::thread mainThread(cStartThread);
    mainThread.detach();
#endif
}

void writelog(const char* pLogMessage, int nLevel)
{
    switch(nLevel)
    {
    case LOG_INFO:
        log_info("%s", pLogMessage);
        break;
    case LOG_DEBUG:
        log_debug("%s", pLogMessage);
        break;
    case LOG_WARNING:
        log_warning("%s", pLogMessage);
        break;
    case LOG_ERROR:
        log_error("%s", pLogMessage);
        break;
    case LOG_FATAL:
        log_fatal("%s", pLogMessage);
        break;
    default:
        log("%s", pLogMessage);
        break;
    }
}


