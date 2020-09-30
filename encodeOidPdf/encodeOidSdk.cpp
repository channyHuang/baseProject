#include "encodeOidSdk.h"

#include <QLibrary>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QIODevice>

//#include "yDotPdfDll.h"

EncodeSDK::EncodeSDK(QObject *parent)
    :m_bLoadSuccess(false), QObject(parent)
{

}

EncodeSDK::~EncodeSDK()
{

}

bool EncodeSDK::load(const QString& qsLibName)
{
    qDebug() << qsLibName;
    QLibrary qLib(qsLibName);
    if (qLib.load()) {
        LOAD_DLLFUN(DLL_dllVersion, "dllVersion");
        LOAD_DLLFUN(DLL_setPatternFormat, "setPatternFormat");
        LOAD_DLLFUN(DLL_pdfCreate, "pdfCreate");
        LOAD_DLLFUN(DLL_pdfRelease, "pdfRelease");
        LOAD_DLLFUN(DLL_ydDrawRect, "ydDrawRect");
        LOAD_DLLFUN(DLL_ydDrawEllipse, "ydDrawEllipse");
        LOAD_DLLFUN(DLL_ydDrawPoly, "ydDrawPoly");
        LOAD_DLLFUN(DLL_writeToTif, "writeToTif");
        LOAD_DLLFUN(DLL_writeToPdf, "writeToPdf");
        m_bLoadSuccess = true;
    } else {
        qDebug() << qLib.errorString();
    }
    return m_bLoadSuccess;
}

void EncodeSDK::writeLog(const QString& qsLog)
{

    QDateTime dtNow = QDateTime::currentDateTime();
    QString qsFileName = "./log/EncodeOidPdf_" + dtNow.toString("yyyy-MM-dd") + ".log";
    QFile qfLog(qsFileName);
    if (qfLog.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "open log file failed " << qfLog.errorString();
        return;
    }
    QString qsLogMsg = dtNow.toString("hh:mm:ss.zzz ") + qsLog + "\r\n";
    qfLog.write(qsLogMsg.toLocal8Bit().data(), qsLogMsg.toLocal8Bit().length());
    qfLog.close();
}

void EncodeSDK::unload()
{
    m_bLoadSuccess = false;
    DLL_dllVersion = nullptr;
    DLL_setPatternFormat = nullptr;
    DLL_pdfCreate = nullptr;
    DLL_pdfRelease = nullptr;
    DLL_ydDrawRect = nullptr;
    DLL_ydDrawEllipse = nullptr;
    DLL_ydDrawPoly = nullptr;
    DLL_writeToTif = nullptr;
    DLL_writeToPdf = nullptr;
}
