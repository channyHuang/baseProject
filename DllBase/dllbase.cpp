#include "dllbase.h"

DllBase::DllBase(QObject *parent)
    : QObject(parent)
    , m_nHandle(-1)
    , m_bPrintLog(false)
{
    m_thread.bind(this);
    m_thread.start();
}

DllBase::~DllBase()
{
    m_thread.quit();
    m_thread.wait();
}

void DllBase::writeLog(const QString &sLog)
{
    if (false == m_bPrintLog) return;

    QDateTime dtNow = QDateTime::currentDateTime();
    QString sFileName = "./Log/DllBase_"+dtNow.toString("yyyyMMdd")+".log";

    QFile qfLog(sFileName);
    if(qfLog.open(QIODevice::WriteOnly|QIODevice::Append)==false)
    {
        qDebug() << "open log file failed";
        return;
    }

    QString sLogMsg = dtNow.toString("hh:mm:ss.zzz")+" "+sLog+"\r\n";
    qfLog.write(sLogMsg.toLocal8Bit().data(),sLogMsg.toLocal8Bit().length());

    qfLog.close();
}

void DllBase::initInThread()
{

}

void DllBase::deinitInThread()
{
    Deinit(m_nHandle);
}

int DllBase::Init(int nType, char *sParas)
{
    QString sIniFileName = ".\\Config\\DllBase.ini";
    QSettings qsSetting(sIniFileName,QSettings::IniFormat);
    m_bPrintLog = (qsSetting.value("/LOG/isPrintLog", 0).toInt() == 0 ? false : true);

    m_Port.setPortName(sParas);
    if (false == m_Port.open(QIODevice::ReadWrite))
        return ERROR_CODE_UNKNOW;

    if (1 == nType)
    {
        m_Port.setBaudRate(QSerialPort::Baud9600);
        m_Port.setParity(QSerialPort::NoParity);
        m_Port.setDataBits(QSerialPort::Data8);
        m_Port.setStopBits(QSerialPort::OneStop);
        m_Port.clearError();
        m_Port.clear();

        connect(&m_Port, SIGNAL(readyRead()), this, SLOT(sltRecvPort()));
    }
    else if (2 == nType)
    {
        connect(&m_SocketServer, SIGNAL(newConnection()), this, SLOT(sltOnNewConnection()));
        connect(&m_SocketServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(sltOnError(QAbstractSocket::SocketError)));
        m_SocketServer.listen();
    }
    else if (3 == nType)
    {
        connect(m_SocketClient, SIGNAL(connected()), this, SLOT(sltOnConnected()));
        connect(m_SocketClient, SIGNAL(disconnected()), this, SLOT(sltOnDisonnected()));
        connect(m_SocketClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sltOnError(QAbstractSocket::SocketError)));
        connect(m_SocketClient, SIGNAL(stateChanged()), this, SLOT(sltOnStatechanged()));

        connect(m_SocketClient, SIGNAL(readyRead()), this, SLOT(sltRecvNet()));

        m_SocketClient.disconnectFromHost();
        m_SocketClient.connectToHost(QHostAddress(m_sIp),m_nPort);
    }
    else return ERROR_CODE_PARAM;

    writeLog(QString().sprintf("Init(%d, %s) success", nType, sParas));
    m_nHandle = 1;
    return m_nHandle;
}

void DllBase::sltOnNewConnection()
{
    writeLog("sltNewConnection");
    QTcpSocket *client = m_SocketServer.nextPendingConnection();
    if (nullptr == client) {
        writeLog("Empty connection");
        return;
    }
    connect(client, SIGNAL(readyRead()), this, SLOT(sltRecvNet()));
    connect(client,SIGNAL(disconnected()),this,SLOT(sltOnDisonnected()));
    m_qlSocketClients.push_back(client);
}

void DllBase::sltOnError(QAbstractSocket::SocketError socketError)
{
    writeLog("sltOnError " + socketError.errorString());
}

int DllBase::Deinit(int nHandle)
{
    if (nHandle != m_nHandle)
    {
        writeLog("Deinit failed, handle error");
        return ERROR_CODE_PARAM;
    }

    m_Port.close();
    m_SocketClient.close();
    m_SocketServer.close();
    m_qlSocketClients.clear();

    m_nHandle = -1;

    return ERROR_CODE_NORMAL;
}

int DllBase::SetCallback(int nHandle, CBFun_Callback pFunc, void* pUser)
{
    if (nHandle != m_nHandle)
    {
        writeLog("SetCallback failed, handle error");
        return ERROR_CODE_PARAM;
    }

    m_pCallbackFunc = pFunc;
    m_pCallbackUser = pUser;

    return ERROR_CODE_NORMAL;
}

void DllBase::sltRecvPort()
{

}

void DllBase::sltRecvNet()
{

}

void DllBase::sltOnConnected()
{

}

void DllBase::sltOnDisonnected()
{
    QTcpSocket *client = dynamic_cast<QTcpSocket*>(sender());
    for (QList<QTcpSocket*>::iterator itr = m_plSocketClients.begin(); itr != m_plSocketClients.end(); itr++)
    {
        if (*itr == client)
        {
            itr = m_plSocketClients.erase(itr);
            break;
        }
    }
}

void DllBase::sltOnStatechanged()
{

}
