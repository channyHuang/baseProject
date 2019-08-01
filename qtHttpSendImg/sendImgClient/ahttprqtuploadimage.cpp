#include "ahttprqtuploadimage.h"

AHttpRqtUploadImage::AHttpRqtUploadImage(QString sRqtUrl, QObject *parent)
    : QObject(parent)
{
    m_sRqtUrl = sRqtUrl;
    //m_sRqtUrl = "http://127.0.0.1:7999";
}

AHttpRqtUploadImage::~AHttpRqtUploadImage()
{

}

void AHttpRqtUploadImage::writeLog(const QByteArray &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();

    if (!QDir().exists("Log"))
        QDir().mkdir("Log");

    QString sFileName = "./Log/httpClientTest_"+dtNow.toString("yyyyMMdd")+".log";

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

int AHttpRqtUploadImage::doHttpRqt(pT_UploadImage stUploadImage, QVariantMap &stResult)
{
    QVariantMap stRsp;
    if(false == checkRqtParam(stUploadImage))
    {
//        stRsp[JsonKey::sCode] = QString::number(PT_INIT_PARA_ERR);
//        stRsp[JsonKey::sMsg] = QString::fromLocal8Bit("Request param error.");
//        makeResult(stRsp,stResult);
//        return PT_INIT_PARA_ERR;
        return 0;
    }

    QString sLogMsg = "doHttpRqt Call Request:" + m_sRqtUrl;
//    writeLog(sLogMsg);

    QElapsedTimer timer;
    timer.start();

    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(QUrl(m_sRqtUrl));
    //request.setRawHeader("Content-Type","application/json;charset=utf-8");

    QHttpMultiPart *multiPart = genHttpBody(stUploadImage);

    QNetworkReply * pReply = manager.post(request, multiPart);

    QEventLoop eventloop;
    connect( pReply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec();

    int iElapsedTime = timer.elapsed();
    sLogMsg = "Call Reply time:" + QString::number(iElapsedTime)+"ms";
//    writeLog(sLogMsg);

    if(false == pReply->isOpen())
    {
//        writeLog("pReply isn't open.");
//        stRsp[JsonKey::sCode] = QString::number(PT_OTHER_ERR);
//        stRsp[JsonKey::sMsg] = QString::fromLocal8Bit("Reply isn't open");
//        makeResult(stRsp,stResult);
//        return PT_OTHER_ERR;
        return 0;
    }
    QByteArray baReply = pReply->readAll();
    writeLog("Reply Data:" + baReply);
    emit sig_recvRsp(baReply);
//    APubTool::parseJsonToMap(baReply,stRsp);
    int iRtn = makeResult(stRsp,stResult);

    pReply->deleteLater();
    pReply = Q_NULLPTR;

    writeLog("doHttpRqt finish.");
    return iRtn;
}

QHttpMultiPart *AHttpRqtUploadImage::genHttpBody(pT_UploadImage stUploadImage)
{
    QJsonObject jsonObj;
    jsonObj.insert("picId", stUploadImage->pidId);
    jsonObj.insert("captureTime", QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss"));
    jsonObj.insert("sequenceNo", stUploadImage->sequenceNo);
    jsonObj.insert("imageParamName", stUploadImage->imageParamName);
    jsonObj.insert("licenseImageParamName", stUploadImage->licenseImageParamName);
    jsonObj.insert("binImageParamName", stUploadImage->binImageParamName);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    //对于参数，头部信息中应至少包含name
    QHttpPart paramPart;
    QString paramHeader = QString().sprintf("form-data; name=\"%s\"", "xxx");
    paramPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(paramHeader));
    paramPart.setBody(jsonDoc.toJson());
    multiPart->append(paramPart);

    //对于图片文件，头部信息中应至少包含name，filename，Content-Type
    QHttpPart imagePart = genImageHttpPart(stUploadImage->imageParamName, stUploadImage->imageName, multiPart);
    multiPart->append(imagePart);
    if (QFile().exists(stUploadImage->licenseImageName))
    {
        QHttpPart licenseImagePart = genImageHttpPart(stUploadImage->licenseImageParamName, stUploadImage->licenseImageName, multiPart);
        multiPart->append(licenseImagePart);
    }
    if (QFile().exists(stUploadImage->binImageName))
    {
        QHttpPart binImagePart = genImageHttpPart(stUploadImage->binImageParamName, stUploadImage->binImageName, multiPart);
        multiPart->append(binImagePart);
    }

    return multiPart;
}

QHttpPart AHttpRqtUploadImage::genImageHttpPart(const char *imageParamName, const char *imageName, QHttpMultiPart *parentPart)
{
    QFileInfo fileInfo(imageName);

    QHttpPart imagePart;
    QString sContentType = "image/" + fileInfo.suffix().toLower();
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(sContentType));
    QString imageHeader = QString().sprintf("form-data; name=\"%s\"; filename=\"%s\"", imageParamName, imageName);
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(imageHeader));
    QFile *file = new QFile(imageName);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(parentPart);

    return imagePart;
}

bool AHttpRqtUploadImage::checkRqtParam(pT_UploadImage stUploadImage)
{
    if (!QFile().exists(stUploadImage->imageName)) return false;
    return true;
}

int AHttpRqtUploadImage::makeResult(const QVariantMap &stReply, QVariantMap &stResult)
{
    (void)stReply;
    (void)stResult;
    return 0;
}
