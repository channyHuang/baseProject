#include "NetworkManager.h"

const QString qsRequestPre = "http://192.168.1.114/";
const QString qsLoginApi = "cgi-bin/testserver.cgi";
const QString qsDeleteFilesApi = "xxx/deleteFiles";
const QString qsUploadFileApi = "xxx/updateFile";

NetworkManager::NetworkManager(QObject* parent) : QObject(parent)
{
    try {
        m_thread.bind(this);
        m_thread.start();
        m_pNetworkManager = new QNetworkAccessManager(parent);
    } catch (std::exception e) {
        CommonFunc::writeLog("NetworkManager create failed " + QString(e.what()));
    }
}

NetworkManager::~NetworkManager() {
    m_thread.quit();
    m_thread.wait();
}

void NetworkManager::initInThread()
{}

void NetworkManager::deinitInThread()
{}

void NetworkManager::webRequest(QString qsUrl, QByteArray qbaBody, bool bGet, int nCookieState) {
    QNetworkRequest qRequest;
    qRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qRequest.setRawHeader("Accept", "application/json");
    if (nCookieState == SetCookie) {
        qRequest.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(m_loginCookie));
    }
    qRequest.setUrl(QUrl(qsRequestPre + qsUrl));

    QNetworkReply* pReply = nullptr;
    if (bGet) {
        pReply = m_pNetworkManager->get(QNetworkRequest(QUrl(qsRequestPre + qsUrl)));
    } else {
        pReply = m_pNetworkManager->post(qRequest, qbaBody);
    }
    CommonFunc::writeLog("sent request to server ");
    CommonFunc::writeLog("webRequest " + QString().sprintf("%p", QThread::currentThread()->currentThreadId()));
    if (pReply) {
        QMetaObject::Connection con = connect(pReply, &QNetworkReply::finished, this, &NetworkManager::sltRequestFinished, Qt::QueuedConnection);
        pReply->setProperty("url", QVariant(qsUrl));
        if (!con) {
           CommonFunc::writeLog("request connect finished failed");
        }
        con = connect(pReply, &QIODevice::readyRead, this, &NetworkManager::httpReadyRead);
        if (!con) {
            CommonFunc::writeLog("request connect readyRead failed");
        }
        connect(pReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
                [=](QNetworkReply::NetworkError code){
            CommonFunc::writeLog("request failed, network error " + code);
        });

        if (nCookieState == SaveCookie) {
            m_loginCookie = m_pNetworkManager->cookieJar()->cookiesForUrl(pReply->url());
            jar = new QNetworkCookieJar;
            jar->setCookiesFromUrl(m_loginCookie, pReply->request().url());
            m_pNetworkManager->setCookieJar(jar);
        }
    }
}

void NetworkManager::sltRequestFinished() {
    QNetworkReply* pReply = dynamic_cast<QNetworkReply*>(sender());
    QByteArray qbaResult = pReply->readAll();

    CommonFunc::writeLog("requestFinish ");
    CommonFunc::writeLog("sltRequestFinished " + QString().sprintf("%p", QThread::currentThread()->currentThreadId()));
    CommonFunc::writeLog(qbaResult);
    QJsonDocument qjdResult = QJsonDocument::fromJson(qbaResult);
    if (!qjdResult.isObject()) {
        //CommonFunc::writeLog(QString().sprintf("not a json string"));
        if (m_pFunc != nullptr) {
            m_pFunc(qbaResult.toStdString(), 0, m_pUser);
        }
        return;
    }
    QJsonObject qjoResult = qjdResult.object();
    int nErrCode = -1;
    QString qsErrMsg;
    if (qjoResult.contains("errorCode")) {
        nErrCode = qjoResult.value("errorCode").toString().toInt();
    }
    if (nErrCode != 0) {
        qsErrMsg = qjoResult.value("errorMsg").toString();
        CommonFunc::writeLog(QString().sprintf("----- request fialed, errorCode = %d, errorMsg = %s", nErrCode, qsErrMsg.toStdString().c_str()));
        if (m_pFunc != nullptr) {
            m_pFunc(qsErrMsg.toStdString(), nErrCode, m_pUser);
        }
    }
    else {
        CommonFunc::writeLog(QString().sprintf("----- request success %s -----", qjoResult.value("msg").toString().toStdString().c_str()));
        //CommonFunc::writeLog(QString(qbaResult));
        if (m_pFunc != nullptr) {
            m_pFunc(qbaResult.toStdString(), nErrCode, m_pUser);
        }
    }
    QString qsReqUrl = pReply->property("url").toString();
    //to_do
    pReply->deleteLater();
}

void NetworkManager::webJsonRequest(QHash<QString, QString> qhBody)
{
    CommonFunc::writeLog("webJsonRequest " + QString().sprintf("%p", QThread::currentThread()->currentThreadId()));
    QJsonObject qjoBody;
    for (QHash<QString, QString>::iterator itr = qhBody.begin(); itr != qhBody.end(); itr++) {
        qjoBody.insert(itr.key(), itr.value());
    }
    QJsonDocument qjdDoc(qjoBody);

    webRequest(qsLoginApi, qjdDoc.toJson(), false, SaveCookie);
}

void NetworkManager::webJsonArrayRequest(QList<QString> qlArrayString) {
    QJsonObject qjoBody;
    QJsonArray qjaIds;
    for (int i = 0; i < qlArrayString.size(); i++) {
        qjaIds.append(QJsonValue::fromVariant(QVariant(qlArrayString[i])));
    }
    qjoBody.insert("fileIds", qjaIds);
    QJsonDocument qjdDoc(qjoBody);
    webRequest(qsDeleteFilesApi, qjdDoc.toJson(), false, SetCookie);
}

void NetworkManager::webHttpMultiRequest(QString qsUrl, QString qsFilePath) {
    QHttpMultiPart* pMultiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart qFilePart;
    qFilePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString().sprintf("form-data;name=\"file\";filename=\"%s\"", qsFilePath.toStdString().c_str())));
    qFilePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    QFile* pFile = new QFile(qsFilePath, pMultiPart);
    pFile->open(QIODevice::ReadOnly);
    qFilePart.setBodyDevice(pFile);
    pMultiPart->append(qFilePart);

    QHttpPart qParamPart;
    qParamPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"pathUrl\""));
    qParamPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    qParamPart.setBody("matrix/file");
    pMultiPart->append(qParamPart);

    QNetworkRequest qRequest;
    //qRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data;"));
    qRequest.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(m_loginCookie));
    qRequest.setUrl(QUrl(qsRequestPre + qsUploadFileApi));

    QNetworkReply* pReply = nullptr;
    //CommonFunc::writeLog(QString().sprintf("----- request upload file -----", qsFilePath.toStdString().c_str()));
    pReply = m_pNetworkManager->post(qRequest, pMultiPart);
    if (pReply) {
        QMetaObject::Connection con = connect(pReply, &QNetworkReply::finished, this, &NetworkManager::sltRequestFinished);
        pReply->setProperty("url", QVariant(qsUploadFileApi));
        pReply->setProperty("fileName", QVariant(qsFilePath));
        if (!con) {
            //CommonFunc::writeLog("request connect finished failed");
        }
        con = connect(pReply, &QIODevice::readyRead, this, &NetworkManager::httpReadyRead);
        if (!con) {
            //CommonFunc::writeLog("request connect readyRead failed");
        }
    }
}

void NetworkManager::test() {
    QUrl url = QUrl::fromUserInput("https://www.qt.io/");
    auto pReply = m_pNetworkManager->get(QNetworkRequest(url));
    if (pReply) 
    {
        connect(pReply, &QNetworkReply::finished, this, &NetworkManager::sltRequestFinished);
        connect(pReply, &QIODevice::readyRead, this, &NetworkManager::httpReadyRead);
    }
}

void NetworkManager::httpReadyRead()
{
    
}

void NetworkManager::login(std::string sUserName, std::string sPassword)
{
    CommonFunc::writeLog("login " + QString().sprintf("%p", QThread::currentThread()->currentThreadId()));
    QHash<QString, QString> qhLoginInfo;
    qhLoginInfo["account"] = QString(sUserName.c_str());
    qhLoginInfo["password"] = CommonFunc::encodePassword(QString(sPassword.c_str()));
    qhLoginInfo["csnid"] = "1";
    qhLoginInfo["deviceid"] = "2";
    qhLoginInfo["location"] = "3";
    NetworkManager::getInstance()->webJsonRequest(qhLoginInfo);
}

void NetworkManager::setCallback(CBFun_NetworkCallback pFunc, void *pUser)
{
    m_pFunc = pFunc;
    m_pUser = pUser;
}

void NetworkManager::sltNetworkError(QNetworkReply::NetworkError code)
{
    CommonFunc::writeLog("network error");
    if (m_pFunc) {
        m_pFunc("network error", 1, m_pUser);
    }
}
