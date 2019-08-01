#ifndef ATCPSERVER_H
#define ATCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDateTime>
#include <QFile>
#include <QDir>

class ATcpServer : public QObject
{
    Q_OBJECT
public:
    explicit ATcpServer(QObject *parent = nullptr);

signals:

public slots:
    void slt_newConnection();
    void slt_recvData();

private:
    void parseData();

private:
    QTcpServer *m_server;
    QTcpSocket *m_client;

    QByteArray baRequest;
    QByteArray baBoundary;
    int nRequestLen;
};

#endif // ATCPSERVER_H
