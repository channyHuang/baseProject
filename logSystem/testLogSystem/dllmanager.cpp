#include "dllmanager.h"

DllManager::DllManager(QObject *parent)
    : QObject(parent)
    , m_nHandle(-1)
    , m_bLoaded(false)
{
    writeLog("start create");
    Dll_Init = nullptr;
    Dll_Log = nullptr;
    writeLog("create");
}

DllManager::~DllManager()
{
    unload();
}

void DllManager::writeLog(const QString &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();
    QString sPath = "log";
    QString sFileName = "./log/DllManagers_"+dtNow.toString("yyyyMMdd")+".log";

    QDir qdir(sPath);
    if (!qdir.exists()) {
        QDir dir;
        dir.mkpath(sPath);
    }

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

bool DllManager::loadDll()
{
    writeLog("start to load dll");
    if (m_bLoaded) return m_bLoaded;

    try
    {
        m_sDllName = "logSystem.dll";
        QLibrary qLib(m_sDllName);
        if (qLib.load())
        {
            Dll_Init = (TYPE_DLL_Init)qLib.resolve("init");
            Dll_Log = (TYPE_DLL_Log)qLib.resolve("writelog");

            if (nullptr == Dll_Init) {
                writeLog("Load Dll_Init failed");
            }
            if (nullptr == Dll_Log) {
                writeLog("Load Dll_WriteLog failed");
            }
            if (nullptr != Dll_Init && nullptr != Dll_Log)
            {
                m_bLoaded = true;
            }
        }
        else
            writeLog(QString().sprintf("Load dll %s failed, error = %s", m_sDllName.toStdString().c_str(), qLib.errorString().toStdString().c_str()));

        writeLog("load dll success");
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

void DllManager::test()
{
    Dll_Init("./log", "mylog", 3);
    Dll_Log("---%s---", 4);
}

void DllManager::unload()
{
    m_bLoaded = false;

    Dll_Init = nullptr;
    Dll_Log = nullptr;
}
