#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <qobject.h>
#include <qfile.h>
#include <qurl.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qnetworkcookie.h>
#include <QtNetwork/qnetworkcookiejar.h>
#include <QtNetwork/qhttpmultipart.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qiodevice.h>
#include <qthread.h>
#include <qvariant.h>
#include <qlist.h>
#include <qhash.h>
#include <qcoreapplication.h>
#include <thread>
#include <fstream>
#include <mutex>

#include "common/common.h"
#include "common/qtEventAndThread.h"

static std::mutex g_mutex;
static QCoreApplication* g_app = QCoreApplication::instance();
static std::thread qtThread;
static QThread qThread;

typedef void (__stdcall *CBFun_NetworkCallback)(std::string sMsg, int nSeq, void* pUser);

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    enum ECookieState {
        SaveCookie = 1, 
        SetCookie, 
        TotalNum
    };

    static NetworkManager* getInstance() {
        static NetworkManager* m_instance = new NetworkManager();
        return m_instance;
    }

    virtual ~NetworkManager();
    void initInThread();
    void deinitInThread();

    void setCallback(CBFun_NetworkCallback pFunc, void *pUser);
    void login(std::string sUserName, std::string sPassword);
    void webRequest(QString qsUrl, QByteArray qbaBody, bool bGet, int nCookieState);
    void webJsonRequest(QHash<QString, QString> qhBody);
    void webJsonArrayRequest(QList<QString> qlArrayString);
    void webHttpMultiRequest(QString qsUrl, QString qsFilePath);
    void test();

public slots:
    void sltRequestFinished();
    void httpReadyRead();
    void sltNetworkError(QNetworkReply::NetworkError code);

private:
    QNetworkAccessManager* m_pNetworkManager;
    QList<QNetworkCookie> m_loginCookie;
    QNetworkCookieJar* jar;

    NetworkManager(QObject* parent = nullptr);
    CBFun_NetworkCallback m_pFunc = nullptr;
    void *m_pUser = nullptr;

    DllThread<NetworkManager> m_thread;
};
#endif // NETWORKMANAGER_H
