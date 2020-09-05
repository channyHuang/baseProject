#include "logThread.h"

LogThread::LogThread(QObject *parent) : QThread(parent) {
    writeBuffer = new LogBuffer();
    writeBuffer->next = writeBuffer;
    writeBuffer->prev = writeBuffer;
    m_nBufferCnt = 1;
    readBuffer = writeBuffer;
}

LogThread::~LogThread() {
}

bool LogThread::init(QString qsLogDir, QString qsLogName) {
    QDir qdir(qsLogDir);
    if (!qdir.exists()) {
        if (!QDir::current().mkdir(qsLogDir)) {
            qDebug() << "make log dir failed, dir = " << qsLogDir;
            return false;
        }
    }
    m_qsLogDir = qsLogDir;
    m_qsLogName = qsLogDir + "/" + qsLogName + "_" + QDate::currentDate().toString("yyyy-MM-dd");
    QFile file;
    if (file.exists(m_qsLogName + ".log")) {
        int index = 1;
        while (file.exists(m_qsLogName + "_" + QString::number(index) + ".log")) index++;
        m_qsLogName += "_" + QString::number(index);
    }
    m_qsLogName += ".log";
    return true;
}

bool LogThread::writeToDisk(QByteArray qsLogContent) {
    QFile file(m_qsLogName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "open log file failed, file name = " << m_qsLogName;
        return false;
    }
    file.write(qsLogContent);
    file.close();
    return true;
}

bool LogThread::append(QString qsLogContent) {
    return append(QByteArray(qsLogContent.toStdString().c_str()));
}

bool LogThread::append(QByteArray qbaLogContent) {
    if (qbaLogContent.length() >= SINGLE_BUFFER_LEN) {
        qDebug() << "log too large to put into buffer";
        return false;
    }
    m_mutex.lock();
    if (writeBuffer->type == Buffer_FULL) {
        if (writeBuffer->next->type == Buffer_FULL) {
            if ((m_nBufferCnt + 1) * SINGLE_BUFFER_LEN >= MAX_MEMORY) {
                qDebug() << "no space left to new a buffer";
                return false;
            }
            LogBuffer *newBuffer = new LogBuffer();
            newBuffer->next = writeBuffer->next;
            newBuffer->prev = writeBuffer;
            writeBuffer->next->prev = newBuffer;
            writeBuffer->next = newBuffer;
            m_nBufferCnt++;
        }
        writeBuffer = writeBuffer->next;
    }
    strncpy(writeBuffer->buffer + writeBuffer->usedLen, qbaLogContent.data(), qbaLogContent.length());
    writeBuffer->usedLen += qbaLogContent.length();
    writeBuffer->type = Buffer_FULL;
    m_mutex.unlock();
    return true;
}

void LogThread::run() {
    while (1) {
        m_mutex.lock();
        if (readBuffer->type == LogBufferType::Buffer_Empty) {
            m_mutex.unlock();
            continue;
        }

        writeToDisk(QByteArray(readBuffer->buffer, readBuffer->usedLen));
        readBuffer->type = Buffer_Empty;
        readBuffer->usedLen = 0;
        readBuffer = readBuffer->next;
        m_mutex.unlock();
    }

}

void LogThread::sltWriteLog(QByteArray qbaLogContent) {
    append(qbaLogContent);
}
