#include "dllmanager.h"

#include "C:/AProjects/csn-edr-my/src/main/csn-windows/ZeroTrustSecureClient/PluginProject/UKeyDll/ukeydll_global.h"

DllManager::DllManager(QObject *parent)
    : QObject(parent)
    , m_nHandle(-1)
    , m_bLoaded(false)
{
    std::ifstream ifs("config", std::ios::in);
    std::string sConfig((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());
    //m_qsDllName = QString::fromLocal8Bit(sConfig.c_str());
    m_qsDllName = "C:/AProjects/csn-edr-my/src/main/csn-windows/ZeroTrustSecureClient/PluginProject/ownlibs/Release/UKeyDll.dll";
    Dll_Init = nullptr;
    Dll_Deinit = nullptr;
    Dll_SetCallback = nullptr;
}

DllManager::~DllManager()
{
    unload();
}

void OnCallback(int nHandle, T_Callback_Struct* pCallbackResult, void *pUser)
{
    if (nullptr == pUser)
    {
        CommonFunc::writeLog(QString("OnCallback failed, pUser is null"));
        return;
    }

    DllManager *dllManager = (DllManager*)pUser;
    //QTimer::singleShot(0, pUser, SLOT(sltOnCallback(pCallbackResult)));
}

void DllManager::sltOnCallback(T_Callback_Struct *pCallbackResult)
{

}

bool DllManager::loadDll()
{
    if (m_bLoaded) return m_bLoaded;

    try
    {
        QLibrary qLib(m_qsDllName);
        if (qLib.load())
        {
            Dll_Init = (TYPE_DLL_Init)qLib.resolve("Init");
            Dll_Deinit = (TYPE_DLL_Deinit)qLib.resolve("Deinit");
            Dll_SetCallback = (TYPE_DLL_SetCallback)qLib.resolve("SetCallback");

            if (nullptr == Dll_Init)
                CommonFunc::writeLog("Load Dll_Init failed");
            if (nullptr == Dll_Deinit)
                CommonFunc::writeLog("Load Dll_Deinit failed");
            if (nullptr == Dll_SetCallback)
                CommonFunc::writeLog("Load Dll_SetCallback failed");

            m_bLoaded = true;
        }
        else
            CommonFunc::writeLog(QString().asprintf("Load dll %s failed, error = %s", m_qsDllName.toStdString().c_str(), qLib.errorString().toStdString().c_str()));

        return m_bLoaded;
    }
    catch(QException &e)
    {
        CommonFunc::writeLog(QString("load dll exception :") + e.what());
        return false;
    }
    catch(...)
    {
        CommonFunc::writeLog(QString("load dll exception :"));
        return false;
    }
}

bool DllManager::loadDllClass() {
    if (m_bLoaded) return m_bLoaded;

    try {
        QLibrary qLib(m_qsDllName);
        if (qLib.load()) {
            CommonFunc::writeLog(QString().asprintf("Load dll %s success", m_qsDllName.toStdString().c_str()));
            typedef IUKeyDll* (*getUKeyDll)();
            getUKeyDll FUNC_getUKeyDll = (getUKeyDll)qLib.resolve("getUKeyDll");

            if (FUNC_getUKeyDll) {
                CommonFunc::writeLog(QString().asprintf("Load function FUNC_getUKeyDll success"));
                IUKeyDll* Class_UKeyDll = FUNC_getUKeyDll();

                if (Class_UKeyDll) {
                    char pUserPin[20];
                    strcpy_s(pUserPin, "wa1234");
                    char* pUKeyNames = new char[1024];
                    bool bHasMultiKey = true;
                    Class_UKeyDll->checkIfHasMultiUKey(pUserPin, pUKeyNames, bHasMultiKey);

                    qDebug() << bHasMultiKey;
                }
            } else {
                CommonFunc::writeLog(QString().asprintf("Load function FUNC_getUKeyDll failed"));
            }
        } else {
            CommonFunc::writeLog(QString().asprintf("Load dll %s failed, error = %s", m_qsDllName.toStdString().c_str(), qLib.errorString().toStdString().c_str()));
        }
        return m_bLoaded;
    } catch(...)
    {
        CommonFunc::writeLog(QString("load dll exception :"));
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
