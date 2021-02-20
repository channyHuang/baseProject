#include "common.h"

CommonFunc::CommonFunc()
{
}

void CommonFunc::writeLog(const QString &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();
    QString sFileName = "./logDemoDll_"+dtNow.toString("yyyyMMdd")+".log";

    QFile qfLog(sFileName);
    if(qfLog.open(QIODevice::WriteOnly|QIODevice::Append)==false)
    {
        qDebug() << "open log file failed";
        return;
    }

    QString sLogMsg = dtNow.toString("hh:mm:ss.zzz")+" "+sLog+"\r\n";
    qfLog.write(sLogMsg.toLocal8Bit().data(),sLogMsg.toLocal8Bit().length());

    qfLog.close();
}

QString CommonFunc::encodePassword(const QString& qsPassword)
{
    QByteArray qbaPassword = qsPassword.toUtf8();
    QString qsEncodePassword = QCryptographicHash::hash(qbaPassword, QCryptographicHash::Md5).toHex();
    return qsEncodePassword;
}
