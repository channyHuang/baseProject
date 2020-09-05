#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QThread>
#include "logThread.h"

enum LOG_LEVEL{LOG_INFO, LOG_DEBUG, LOG_WARNING, LOG_ERROR, LOG_FATAL, LOG_NOLEVEL};

class LogManager : public QObject
{
    Q_OBJECT
public:
    static LogManager* getInstance() {
        static LogManager* m_instance = new LogManager();
        return m_instance;
    }

    virtual ~LogManager();
    void writeLog(const QString &sLog, LOG_LEVEL eLogLevel = LOG_NOLEVEL);

signals:
    void sigWriteLog(QByteArray qbaLogContent);

private:
    LogManager(QObject *parent = nullptr);

    LogThread m_logThread;
    int m_eLogLevel = LOG_NOLEVEL;
    QStringList qslLogLevel = {"INFO", "DEBUG", "WARNING", "ERROR", "FATAL", "NOLEVEL"};
};

#endif // LOGMANAGER_H
