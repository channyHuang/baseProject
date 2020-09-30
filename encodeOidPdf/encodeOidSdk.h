#ifndef ENCODESDK_H
#define ENCODESDK_H

#include <QObject>
#include "windows.h"

#define LOAD_DLLFUN(FUNNAME, FUN_STR) \
    FUNNAME = (TYPE_##FUNNAME) qLib.resolve(FUN_STR); \
    if (!FUNNAME) { writeLog(QString().sprintf("load %s failed", FUN_STR)); return false;} \
    else {writeLog(QString().sprintf("load %s success", FUN_STR));}

#define SetPgsRange  	0
#define SetPgsPos	 	1
#define SetPgsPosAdd 	2
typedef void (WINAPI *ProgressVnt)(int type,int pos,DWORD data);

typedef bool __stdcall (*TYPE_DLL_dllVersion)(int *mainVer,int *subVer);
typedef bool __stdcall (*TYPE_DLL_setPatternFormat)(long dpi,long dotSzX,long dotSzY,bool bBackCode);
typedef void __stdcall (*TYPE_DLL_setRunProgress)(ProgressVnt prog,DWORD data);
typedef DWORD __stdcall (*TYPE_DLL_pdfCreate)(const char *jpgImagePathName);
typedef bool __stdcall (*TYPE_DLL_pdfRelease)(DWORD hPdf);

typedef bool __stdcall (*TYPE_DLL_ydDrawRect)(DWORD hPdf,long id,long left,long top,long right,long bottom,bool bFrame,bool bNumber);
typedef bool __stdcall (*TYPE_DLL_ydDrawEllipse)(DWORD hPdf,long id,long left,long top,long right,long bottom,bool bFrame,bool bNumber);
typedef bool __stdcall (*TYPE_DLL_ydDrawPoly)(DWORD hPdf,long id,long *pts,int ptsCnt,bool bFrame,bool bNumber);

typedef bool __stdcall (*TYPE_DLL_writeToTif)(DWORD hPdf,const char *tifPathName);
typedef bool __stdcall (*TYPE_DLL_writeToPdf)(DWORD hPdf,const char *pdfPathName);

//----------------------------------
typedef DWORD __stdcall (*TYPE_DLL_bookCreate)();
typedef bool __stdcall (*TYPE_DLL_bookAdd)(DWORD hBook,const char *pdfPathName);
typedef bool __stdcall (*TYPE_DLL_releaseBook)(DWORD hBook);
typedef bool __stdcall (*TYPE_DLL_writeToBook)(DWORD hBook,const char *pdfPathName);
typedef void __stdcall (*TYPE_DLL_stopWriteToBook)();

class EncodeSDK : public QObject
{
    Q_OBJECT
public:
    enum ERROR_CODE {LOAD_SUCCESS, LOAD_FAILED, FUNC_NOT_EXIST, UNKNOW_ERROR_CODE};

    explicit EncodeSDK(QObject *parent = nullptr);
    virtual ~EncodeSDK();

    bool load(const QString& qsLibName);
    void writeLog(const QString& qsLog);
    void unload();

    TYPE_DLL_dllVersion DLL_dllVersion = nullptr;
    TYPE_DLL_setPatternFormat DLL_setPatternFormat = nullptr;
    TYPE_DLL_pdfCreate DLL_pdfCreate = nullptr;
    TYPE_DLL_pdfRelease DLL_pdfRelease = nullptr;
    TYPE_DLL_ydDrawRect DLL_ydDrawRect = nullptr;
    TYPE_DLL_ydDrawEllipse DLL_ydDrawEllipse = nullptr;
    TYPE_DLL_ydDrawPoly DLL_ydDrawPoly = nullptr;
    TYPE_DLL_writeToTif DLL_writeToTif = nullptr;
    TYPE_DLL_writeToPdf DLL_writeToPdf = nullptr;
signals:

public slots:

private:

    bool m_bLoadSuccess;


};

#endif // ENCODESDK_H
