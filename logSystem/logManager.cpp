#include "logManager.h"

#include "readini.h"

#include <QTimer>

LogManager::LogManager(QObject *parent) : QObject(parent)
{
    connect(this, &LogManager::sigWriteLog, &m_logThread, &LogThread::sltWriteLog);

    ReadIni readini;
    readini.setIniName("./logSystem.ini");
    QString qsLogLevel = readini.getValue("Log", "logLevel", "");
    m_eLogLevel = (qsLogLevel.isEmpty() ? LOG_NOLEVEL : qsLogLevel.toInt());
    m_logThread.init(readini.getValue("Log", "logPath", "./"), readini.getValue("Log", "logName", "logSystem"));
    m_logThread.start();
}

LogManager::~LogManager()
{

}

void LogManager::writeLog(const QString &sLog, LOG_LEVEL eLogLevel)
{
    if (eLogLevel > m_eLogLevel) return;
    QString sLogMsg = QTime::currentTime().toString("hh:mm:ss.zzz")+" [" + qslLogLevel[eLogLevel] + "] "+sLog+"\r\n";
    emit sigWriteLog(QByteArray(sLogMsg.toStdString().c_str()));
}

