#ifndef DLLMANAGER_H
#define DLLMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QIODevice>
#include <QLibrary>
#include <QException>
#include <QTimer>

#pragma pack(push)
#pragma pack(1)
typedef struct Callback_Struct
{
    int length;
    char data[20];
    int vlpCarClass;
}T_Callback_Struct;
#pragma pack(pop)

typedef void (*CBFun_Callback)(int nHandle, T_Callback_Struct* pCallbackResult, void* pUser);

typedef int __stdcall (*TYPE_DLL_Init)(int nType, char* sParas);
typedef int __stdcall (*TYPE_DLL_Deinit)();
typedef int __stdcall (*TYPE_DLL_SetCallback)(int nHandle, CBFun_Callback pFunc, void* pUser);

class DllManager : public QObject
{
    friend void OnCallback(int nHandle, T_Callback_Struct* pVlpResult, void* pUser);

    Q_OBJECT
public:
    explicit DllManager(QObject *parent = 0);
    virtual ~DllManager();

private:
    bool loadDll();
    void unload();

    void writeLog(const QString &sLog);

private slots:
    void sltOnCallback(T_Callback_Struct* pCallbackResult);

private:
    TYPE_DLL_Init Dll_Init;
    TYPE_DLL_Deinit Dll_Deinit;
    TYPE_DLL_SetCallback Dll_SetCallback;

    QString m_sDllName;
    int m_nHandle;
    bool m_bLoaded;
};

#endif // DLLMANAGER_H
