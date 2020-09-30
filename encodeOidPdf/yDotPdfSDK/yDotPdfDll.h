
#ifndef __yDotPdfDllExp___H
#define __yDotPdfDllExp___H

#ifdef YDOTDLL_EXPORTS
#define YDOTDLL_API __declspec(dllexport)
#else
#define YDOTDLL_API __declspec(dllimport)
#endif

#define OP_API WINAPI

#define SetPgsRange  	0
#define SetPgsPos	 	1
#define SetPgsPosAdd 	2
typedef void (WINAPI *ProgressVnt)(int type,int pos,DWORD data);

namespace YdOT
{
	YDOTDLL_API BOOL OP_API dllVersion(int *mainVer,int *subVer);

	YDOTDLL_API BOOL	OP_API setPatternFormat(long dpi,long dotSzX,long dotSzY,BOOL bBackCode);
	YDOTDLL_API void	OP_API setRunProgress(ProgressVnt prog,DWORD data);
	YDOTDLL_API DWORD	OP_API pdfCreate	(const char *jpgImagePathName);
	YDOTDLL_API BOOL	OP_API pdfRelease	(DWORD hPdf);

	YDOTDLL_API BOOL	OP_API ydDrawRect	(DWORD hPdf,long id,long left,long top,long right,long bottom,BOOL bFrame,BOOL bNumber);
	YDOTDLL_API BOOL	OP_API ydDrawEllipse(DWORD hPdf,long id,long left,long top,long right,long bottom,BOOL bFrame,BOOL bNumber);
	YDOTDLL_API BOOL	OP_API ydDrawPoly	(DWORD hPdf,long id,long *pts,int ptsCnt,BOOL bFrame,BOOL bNumber);

	YDOTDLL_API BOOL	OP_API	writeToTif(DWORD hPdf,const char *tifPathName);
	YDOTDLL_API BOOL	OP_API	writeToPdf(DWORD hPdf,const char *pdfPathName);

	//----------------------------------
	YDOTDLL_API DWORD	OP_API bookCreate	();
	YDOTDLL_API BOOL	OP_API bookAdd		(DWORD hBook,const char *pdfPathName);
	YDOTDLL_API BOOL	OP_API releaseBook	(DWORD hBook);
	YDOTDLL_API BOOL	OP_API writeToBook	(DWORD hBook,const char *pdfPathName);
	YDOTDLL_API void	OP_API stopWriteToBook();
}

///////////////////////////////////////////
#define RtErr_None				0

////////////////////////////////////

#endif