#ifndef DLLBASE_H
#define DLLBASE_H

#include "dllbase_global.h"
#include "qteventandthread.h"

#include <QTcpSocket>
#include <QTcpServer>
#include <QSerialPort>
#include <QDateTime>
#include <QFile>
#include <QIODevice>
#include <QException>
#include <QSettings>
#include <QList>

enum
{
    ERROR_CODE_UNKNOW = -2000,
    ERROR_CODE_PARAM = -1000,
    ERROR_CODE_NORMAL = 0
};

#pragma pack(push)
#pragma pack(1)
typedef struct Callback_Struct
{
    int length;
    char data[20];
    int vlpCarClass;
}T_Callback_Struct;
#pragma pack(pop)

typedef void (*CBFun_Callback)(int nHandle, T_Callback_Struct* pCallbackResult, void* pUser);

class DllBase : public QObject
{
    Q_OBJECT
public:
    DllBase(QObject *parent = 0);
    virtual ~DllBase();

    void initInThread();
    void deinitInThread();

    int Init(int nType,char* sParas );
    int Deinit(int nHandle);
    int SetCallback(int nHandle, CBFun_Callback pFunc, void* pUser);

private:
    void writeLog(const QString &sLog);

private slots:
    void sltRecvPort();
    void sltRecvNet();
    void sltOnConnected();
    void sltOnDisonnected();
    void sltOnStatechanged();
    void sltOnNewConnection();
    void sltOnError(QAbstractSocket::SocketError socketError);

private:
    bool m_bPrintLog;
    int m_nHandle;

    CBFun_Callback m_pCallbackFunc;
    void *m_pCallbackUser;

    QSerialPort m_Port;
    QTcpSocket m_SocketClient;
    QTcpServer m_SocketServer;
    QList<QTcpSocket> m_qlSocketClients;

    QString m_sIp;
    int m_nPort;

    DllThread<DllBase> m_thread;
};

#endif // DLLBASE_H
