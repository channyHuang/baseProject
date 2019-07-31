#include <QCoreApplication>
#include <QImage>
#include <QDebug>
#include <QDateTime>

#include <iostream>
#include <memory>
#include "http_server.h"

#include <QFile>
#include <QDateTime>
#include <QDir>

// 初始化HttpServer静态类成员
mg_serve_http_opts HttpServer::s_server_option;
std::string HttpServer::s_web_dir = "./web";
std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;
std::unordered_set<mg_connection *> HttpServer::s_websocket_session_set;

void writeLog(const QByteArray &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();

    if (!QDir().exists("Log"))
        QDir().mkdir("Log");

    QString sFileName = "./Log/httpServerTest_"+dtNow.toString("yyyyMMdd")+".log";

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

bool receive_image(std::string url, const char *body, int body_len, mg_connection *c, OnRspCallback rsp_callback)
{
    // do sth
    std::cout << "handle fun1" << std::endl;
    std::cout << "url: " << url << std::endl;
    //std::cout << "body: " << body << std::endl;

    QByteArray baBody = QByteArray(body, body_len);

    int pos = baBody.indexOf("\r\n\r\n");
    int len = baBody.left(pos).toInt();

    writeLog("receive image, image length = " + QByteArray::number(len));
    writeLog("image data = " + baBody.mid(pos + 4).toHex());

    QImage img;
    img.loadFromData(baBody.mid(pos + 4));
    QDateTime now = QDateTime::currentDateTime();
    img.save(QString().sprintf("saveimg_%s.jpeg", now.toString("yyyyMMdd_hhmmss.zzz").toStdString().c_str()));

    rsp_callback(c, "rsp1");

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string port = "7999";
    auto http_server = std::shared_ptr<HttpServer>(new HttpServer);
    http_server->Init(port);
    // add handler
    http_server->AddHandler("/api/sendimg", receive_image);
    http_server->Start();

    return a.exec();
}
