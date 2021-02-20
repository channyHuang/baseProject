#include "SerailManager.h"

SerailManager* SerailManager::m_instance;

SerailManager::SerailManager(QObject *parent)
    : QObject(parent)
    , m_nHandle(-1)
    , m_bPrintLog(false)
{
    m_thread.bind(this);
    m_thread.start();
}

SerailManager::~SerailManager()
{
    m_thread.quit();
    m_thread.wait();
}

void SerailManager::initInThread()
{

}

void SerailManager::deinitInThread()
{
    Deinit(m_nHandle);
}

int SerailManager::Init(int nType, char *sParas)
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
        connect(&m_SocketClient, &QTcpSocket::connected, this, &SerailManager::sltOnConnected);
        connect(&m_SocketClient, &QTcpSocket::disconnected, this, &SerailManager::sltOnDisonnected);
        connect(&m_SocketClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
              [=](QAbstractSocket::SocketError socketError){
            /* ... */
            CommonFunc::writeLog("sltOnError " + socketError);
        });
        connect(&m_SocketClient, &QTcpSocket::stateChanged, this, &SerailManager::sltOnStatechanged);
        connect(&m_SocketClient, &QTcpSocket::readyRead, this, &SerailManager::sltRecvNet);

        m_SocketClient.disconnectFromHost();
        m_SocketClient.connectToHost(QHostAddress(m_sIp),m_nPort);
    }
    else return ERROR_CODE_PARAM;

    CommonFunc::writeLog(QString().sprintf("Init(%d, %s) success", nType, sParas));
    m_nHandle = 1;
    return m_nHandle;
}

void SerailManager::sltOnNewConnection()
{
    CommonFunc::writeLog("sltNewConnection");
    QTcpSocket *client = m_SocketServer.nextPendingConnection();
    if (nullptr == client) {
        CommonFunc::writeLog("Empty connection");
        return;
    }
    connect(client, SIGNAL(readyRead()), this, SLOT(sltRecvNet()));
    connect(client,SIGNAL(disconnected()),this,SLOT(sltOnDisonnected()));
    m_qlSocketClients.push_back(client);
}

int SerailManager::Deinit(int nHandle)
{
    if (nHandle != m_nHandle)
    {
       CommonFunc::writeLog("Deinit failed, handle error");
        return ERROR_CODE_PARAM;
    }

    m_Port.close();
    m_SocketClient.close();
    m_SocketServer.close();
    m_qlSocketClients.clear();

    m_nHandle = -1;

    return ERROR_CODE_NORMAL;
}

int SerailManager::SetCallback(int nHandle, CBFun_Callback pFunc, void* pUser)
{
    if (nHandle != m_nHandle)
    {
        CommonFunc::writeLog("SetCallback failed, handle error");
        return ERROR_CODE_PARAM;
    }

    m_pCallbackFunc = pFunc;
    m_pCallbackUser = pUser;

    return ERROR_CODE_NORMAL;
}

void SerailManager::sltRecvPort()
{

}

void SerailManager::sltRecvNet()
{

}

void SerailManager::sltOnConnected()
{

}

void SerailManager::sltOnDisonnected()
{
    QTcpSocket *client = dynamic_cast<QTcpSocket*>(sender());
    for (QList<QTcpSocket*>::iterator itr = m_qlSocketClients.begin(); itr != m_qlSocketClients.end(); itr++)
    {
        if (*itr == client)
        {
            itr = m_qlSocketClients.erase(itr);
            break;
        }
    }
}

void SerailManager::sltOnStatechanged()
{

}
