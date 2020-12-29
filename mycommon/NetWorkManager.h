#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>

class NetWorkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkManager(QObject *parent = nullptr);
    virtual ~NetWorkManager();

    bool init(bool bServer = false);

signals:

public slots:
    void newConnection();
    void sltReceivedFromClient();
    void sltClientDisconnected();
    void sltServerTimer();

    void connected();
    void sltReceivedFromServer();
    void sltServerDisconnected();
    void sltClientTimer();

private:
    void sentToClient(QTcpSocket *client);
    void sentToServer();

    QTcpServer *qServer;
    QTcpSocket *qSocket;

    QSet<QTcpSocket*> mSetClients;

    QTimer serverTimer;
    QTimer clientTimer;
};

#endif // NETWORKMANAGER_H
