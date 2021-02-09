#ifndef DLLMANAGER_H
#define DLLMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QIODevice>
#include <QLibrary>
#include <QException>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <fstream>

typedef void __stdcall (*TYPE_DLL_Init)(const char* pLogPath, const char* pLogName, int nLevel);
typedef void __stdcall (*TYPE_DLL_Log)(const char* pLogMessage, int nLevel);

class DllManager : public QObject
{
    Q_OBJECT
public:
    explicit DllManager(QObject *parent = 0);
    virtual ~DllManager();


    bool loadDll();
    void unload();
    void test();

    void writeLog(const QString &sLog);

private slots:

private:
    TYPE_DLL_Init Dll_Init;
    TYPE_DLL_Log Dll_Log;

    QString m_sDllName;
    int m_nHandle;
    bool m_bLoaded;
};

#endif // DLLMANAGER_H
