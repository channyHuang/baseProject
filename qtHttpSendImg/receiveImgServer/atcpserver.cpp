#include "atcpserver.h"

ATcpServer::ATcpServer(QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer();
    if (m_server->listen(QHostAddress::Any, 7999))
        qDebug() << "start listen";

    connect(m_server, &QTcpServer::newConnection, this, &ATcpServer::slt_newConnection);
}

void ATcpServer::slt_newConnection()
{
    m_client = m_server->nextPendingConnection();
    connect(m_client, &QTcpSocket::readyRead, this, &ATcpServer::slt_recvData);
}

void writeLog(const QByteArray &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();

    if (!QDir().exists("Log"))
        QDir().mkdir("Log");

    QString sFileName = "./Log/receiveImgServer"+dtNow.toString("yyyyMMdd")+".log";

    QFile qfLog(sFileName);
    if(qfLog.open(QIODevice::WriteOnly|QIODevice::Append)==false)
    {
        qDebug() << "open log file failed";
        return;
    }

    QByteArray sLogMsg = QByteArray(dtNow.toString("hh:mm:ss.zzz").toStdString().c_str()) + " "+sLog+"\r\n";
    qfLog.write(sLogMsg.data(),sLogMsg.length());

    qfLog.close();
}

void ATcpServer::slt_recvData()
{
    QByteArray qbaRecvData = m_client->readAll();
    if (qbaRecvData.length() == 0) return;
    //qDebug() << qbaRecvData;
    writeLog(qbaRecvData);
    writeLog("----------------------------------------------------------------");
    //writeLog(qbaRecvData.toHex());

    if (qbaRecvData.startsWith("POST")) {
        baRequest.clear();
        baRequest.append(qbaRecvData);

        int sPos = baRequest.indexOf("boundary=\"");
        int ePos = baRequest.indexOf("\"", sPos + 10);
        baBoundary = baRequest.mid(sPos + 10, ePos - sPos - 10);
        qDebug() << baBoundary;

        sPos = baRequest.indexOf("Content-Length: ");
        ePos = baRequest.indexOf("\r", sPos);
        nRequestLen = baRequest.mid(sPos + 16, ePos - sPos - 16).toInt();

        qDebug() << nRequestLen;


    } else baRequest.append(qbaRecvData);

    if (baRequest.length() == nRequestLen) {
        nRequestLen = 0;
        parseData();
    }
}

void ATcpServer::parseData()
{

}
