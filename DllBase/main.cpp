#include "dllbase.h"

QtEvent g_qtEevent;
DllBase *g_pDllBase;

void writeLog(const QString &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();
    QString sFileName = "./Log/DllBase_"+dtNow.toString("yyyyMMdd")+".log";

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

int Init( int nType,char* sParas)
{
    if (nullptr == sParas)
        return ERROR_CODE_PARAM;

    try
    {
        if(nullptr == g_pDllBase)
            g_pDllBase = new DllBase();
        if(g_pDllBase) return g_pDllBase->Init(nType, sParas);
    }
    catch(QException &e)
    {
        writeLog(QString("Init exception :") + e.what());
        return ERROR_CODE_UNKNOW;
    }
    catch(...)
    {
        return ERROR_CODE_UNKNOW;
    }

    return ERROR_CODE_UNKNOW;
}

int Deinit(int nHandle)
{
    if(g_pDllBase)
    {
        g_pDllBase->Deinit(nHandle);
        delete g_pDllBase;
        g_pDllBase = nullptr;
        return ERROR_CODE_NORMAL;
    }
    return ERROR_CODE_UNKNOW;
}

int SetCallback(int nHandle, CBFun_Callback pFunc, void* pUser)
{
    if (nullptr == pUser)
        return ERROR_CODE_PARAM;

    if(g_pDllBase) return g_pDllBase->SetCallback(nHandle, pFunc, pUser);
}
