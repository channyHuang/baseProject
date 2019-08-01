#ifndef AHTTPRQTUPLOADIMAGE_H
#define AHTTPRQTUPLOADIMAGE_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QDir>
#include <QEventLoop>
#include <QDateTime>
#include <QElapsedTimer>
#include <QVariantMap>

typedef struct _T_UploadImage {
    char pidId[36];
    char captureTime[20];
    char sequenceNo[2];
    char imageParamName[50];
    char licenseImageParamName[50];
    char binImageParamName[50];
    char imageName[20];
    char licenseImageName[20];
    char binImageName[20];
}T_UploadImage, *pT_UploadImage;

class AHttpRqtUploadImage : public QObject
{
    Q_OBJECT

public:
    explicit AHttpRqtUploadImage(QString sRqtUrl, QObject *parent = nullptr);
    virtual ~AHttpRqtUploadImage();

    virtual int doHttpRqt(pT_UploadImage stUploadImage, QVariantMap &stResult);

protected:
    virtual void writeLog(const QByteArray &sLog);
    virtual bool checkRqtParam(pT_UploadImage stUploadImage);
    virtual int  makeResult(const QVariantMap &stReply,QVariantMap &stResult);

signals:
    void sig_recvRsp(QByteArray baReply);

private:
    QHttpMultiPart* genHttpBody(pT_UploadImage stUploadImage);
    QHttpPart genImageHttpPart(const char *imageParamName, const char *imageName, QHttpMultiPart *parentPart);

private:
    QString m_sRqtUrl;
};

#endif // AHTTPRQTUPLOADIMAGE_H
