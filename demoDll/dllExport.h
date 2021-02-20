#ifndef DLLEXPORT_H
#define DLLEXPORT_H

#include <string.h>
#include <string>

#ifdef __cplusplus
#define D_EXTERN_C extern "C"
#else
#define D_EXTERN_C
#endif

#ifdef __SHARE_EXPORT
#define D_SHARE_EXPORT D_DECL_EXPORT
#else
#define D_SHARE_EXPORT D_DECL_IMPORT
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
#define D_CALLTYPE __stdcall
#define D_DECL_EXPORT __declspec(dllexport)
#define D_DECL_IMPORT
#else
#define __stdcall
#define D_CALLTYPE
#define D_DECL_EXPORT __attribute__((visibility("default")))
#define D_DECL_IMPORT __attribute__((visibility("default")))
#endif

//param void, return void
D_EXTERN_C D_SHARE_EXPORT void D_CALLTYPE funParamVoidReturnVoid();
//param valid, return void
D_EXTERN_C D_SHARE_EXPORT void D_CALLTYPE funParamValidReturnVoid(const char* pString, int nNumber);
//param void, return valid
D_EXTERN_C D_SHARE_EXPORT char* D_CALLTYPE funParamVoidReturnValid();
//param valid, return valid
D_EXTERN_C D_SHARE_EXPORT int D_CALLTYPE funParamValidReturnValid(const char* pString, int nNumber);

//has callback
typedef void (__stdcall *CBFun_NetworkCallback)(std::string sMsg, int nSeq, void* pUser);
D_EXTERN_C D_SHARE_EXPORT int D_CALLTYPE funSetCallback(CBFun_NetworkCallback pFunc, void *pUser);
D_EXTERN_C D_SHARE_EXPORT void D_CALLTYPE funRequestNetworkAndCallback(std::string sUserName, std::string sPassword);


D_EXTERN_C D_SHARE_EXPORT int D_CALLTYPE Init(int nType,char* sParas );
D_EXTERN_C D_SHARE_EXPORT int D_CALLTYPE Deinit(int nHandle);

#endif // DLLEXPORT_H
