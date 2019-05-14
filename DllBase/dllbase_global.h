#ifndef DLLBASE_GLOBAL_H
#define DLLBASE_GLOBAL_H

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

D_EXTERN_C D_SHARE_EXPORT int D_CALLTYPE Init(int nType,char* sParas );
D_EXTERN_C D_SHARE_EXPORT int D_CALLTYPE Deinit(int nHandle);

#endif // DLLBASE_GLOBAL_H
