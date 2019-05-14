#include "dllmanager.h"

DllManager::DllManager(QObject *parent)
    : QObject(parent)
    , m_nHandle(-1)
    , m_bLoaded(false)
{
    Dll_Init = nullptr;
    Dll_Deinit = nullptr;
    Dll_SetCallback = nullptr;
}

DllManager::~DllManager()
{
    unload();
}

void DllManager::writeLog(const QString &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();
    QString sFileName = "./Log/DllManager_"+dtNow.toString("yyyyMMdd")+".log";

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

void OnCallback(int nHandle, T_Callback_Struct* pCallbackResult, void *pUser)
{
    if (nullptr == pUser)
    {
        writeLog(QString("OnCallback failed, pUser is null"));
        return;
    }

    DllManager *dllManager = (DllManager*)pUser;
    QTimer::singleShot(0, pUser, SLOT(sltOnCallback(pCallbackResult)));
}

void DllManager::sltOnCallback(T_Callback_Struct *pCallbackResult)
{

}

bool DllManager::loadDll()
{
    if (m_bLoaded) return;

    try
    {
        QLibrary qLib(m_sDllName);
        if (qLib.load())
        {
            Dll_Init = (TYPE_DLL_Init)qLib.resolve("Init");
            Dll_Deinit = (TYPE_DLL_Deinit)qLib.resolve("Deinit");
            Dll_SetCallback = (TYPE_DLL_SetCallback)qLib.resolve("SetCallback");

            if (nullptr == Dll_Init)
                writeLog("Load Dll_Init failed");
            if (nullptr == Dll_Deinit)
                writeLog("Load Dll_Deinit failed");
            if (nullptr == Dll_SetCallback)
                writeLog("Load Dll_SetCallback failed");

            m_bLoaded = true;
        }
        else
            writeLog(QString().sprintf("Load dll %s failed, error = %s", m_sDllName, qLib.errorString()));

        return m_bLoaded;
    }
    catch(QException &e)
    {
        writeLog(QString("load dll exception :") + e.what());
        return false;
    }
    catch(...)
    {
        writeLog(QString("load dll exception :"));
        return false;
    }
}

void DllManager::unload()
{
    m_bLoaded = false;

    Dll_Init = nullptr;
    Dll_Deinit = nullptr;
    Dll_SetCallback = nullptr;
}
