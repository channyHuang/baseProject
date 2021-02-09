#ifndef LOGTHREAD_H
#define LOGTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>
#include <QTime>
#include <QBuffer>
#include <QException>
#include <QDebug>

#define SINGLE_BUFFER_LEN 10*1024*1024 //10MB
#define MAX_MEMORY 1024*1024*1024 //1G
#define BUFFER_CNT

enum LogBufferType {Buffer_FULL, Buffer_Empty, Buffer_Type};

class LogBuffer
{
public:
    LogBuffer() {
        try {
            buffer = new char[SINGLE_BUFFER_LEN];
            usedLen = 0;
            prev = nullptr;
            next = nullptr;
            type = Buffer_Empty;
        } catch (QException e) {
            qDebug() << "new buffer failed";
            exit(1);
        }
    }

    ~LogBuffer() {
        delete buffer;
    }

    char *buffer;
    qint64 usedLen;
    LogBufferType type;

    LogBuffer *prev;
    LogBuffer *next;
};

class LogThread : public QThread
{
    Q_OBJECT
public:
    explicit LogThread(QObject *parent = nullptr);
    virtual ~LogThread();

    bool init(QString qsLogDir, QString qsLogName);

signals:

public slots:
    void sltWriteLog(QByteArray qbaLogContent);

protected:
    void run() override;

private:
    bool writeToDisk(QByteArray qsLogContent);
    bool append(QByteArray qbaLogContent);
    bool append(QString qsLogContent);

    QMutex m_mutex;

    QString m_qsLogDir;
    QString m_qsLogName;

    LogBuffer *writeBuffer;
    LogBuffer *readBuffer;

    int m_nBufferCnt;
};

#endif // LOGTHREAD_H
