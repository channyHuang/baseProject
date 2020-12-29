#include <QProcess>
#include <QStringList>
#include "NetWorkManager.h"

#include <iostream>
using namespace std;

NetWorkManager::NetWorkManager(QObject *parent) : QObject(parent)
{

}

NetWorkManager::~NetWorkManager() {}

bool NetWorkManager::init(bool bServer) {
    if (bServer) {
        qServer = new QTcpServer();
        connect(qServer, &QTcpServer::newConnection, this, &NetWorkManager::newConnection);

        if (!qServer->listen(QHostAddress::Any, 1234)) {
            cout << "listen failed" << endl;
            return false;
        }
        cout << "listening..." << endl;
        connect(&serverTimer, &QTimer::timeout, this, &NetWorkManager::sltServerTimer);
        serverTimer.start(10000);
    } else {
        qSocket = new QTcpSocket();
        qSocket->connectToHost("127.0.0.1", 1234);
        connect(qSocket, &QTcpSocket::connected, this, &NetWorkManager::connected);
        connect(qSocket, &QTcpSocket::readyRead, this, &NetWorkManager::sltReceivedFromServer);
        connect(qSocket, &QTcpSocket::disconnected, this, &NetWorkManager::sltClientDisconnected);
        cout << "new client" << endl;
        connect(&clientTimer, &QTimer::timeout, this, &NetWorkManager::sltClientTimer);
        clientTimer.start(10000);
    }
    return true;
}

//server
void NetWorkManager::newConnection() {
    QTcpSocket *client = qServer->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &NetWorkManager::sltReceivedFromClient);
    connect(client, &QTcpSocket::disconnected, this, &NetWorkManager::sltClientDisconnected);
    cout << "new Connection " << endl;
    mSetClients.insert(client);
}

void NetWorkManager::sltReceivedFromClient() {
    QTcpSocket *client = static_cast<QTcpSocket *>(sender());
    char *qsReceivedData = new char[4096];
    client->read(qsReceivedData, 4096);
    cout << "server receive " << qsReceivedData << endl;
}

void NetWorkManager::sltServerTimer() {
    foreach (QTcpSocket *client, mSetClients) {
        sentToClient(client);
    }
    cout << "sltServerTimer" << endl;
}

void NetWorkManager::sltClientDisconnected() {
    QTcpSocket *client = static_cast<QTcpSocket *>(sender());
    mSetClients.remove(client);
}

void NetWorkManager::sentToClient(QTcpSocket *client) {
    QString qsSendData = "hello client";
    qint64 nRes = -1;
    if ((nRes = client->write(qsSendData.toStdString().c_str(), qsSendData.length())) < 0) {
        cout << "sent to client failed" << endl;
        return;
    }
    cout << "sent to client success" << endl;
}

//client
void NetWorkManager::connected() {
    cout << "client connected" << endl;
}

void NetWorkManager::sltReceivedFromServer() {
    char *qsReceivedData = new char[4096];
    qSocket->read(qsReceivedData, 4096);
    cout << "client receive " << qsReceivedData << endl;
}

void NetWorkManager::sentToServer() {
    QString qsSendData = "hello server";
    qint64 nRes = -1;
    if ((nRes = qSocket->write(qsSendData.toStdString().c_str(), qsSendData.length())) < 0) {
        cout << "sent to server failed" << endl;
        return;
    }
    cout << "sent to server success" << endl;
}

void NetWorkManager::sltServerDisconnected() {
    cout << "client disconnected" << endl;
}

void NetWorkManager::sltClientTimer() {
    sentToServer();
}
