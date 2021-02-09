#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <string>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */
    __declspec(dllexport) void init(const char* pLogPath, const char* pLogName, int nLevel);
    __declspec(dllexport) void writelog(const char* pLogMessage, int nLevel);
    /*
    __declspec(dllexport) void log(const char* format, ...);
    __declspec(dllexport) void log_info(const char* format, ...);
    __declspec(dllexport) void log_debug(const char* format, ...);
    __declspec(dllexport) void log_warning(const char* format, ...);
    __declspec(dllexport) void log_error(const char* format, ...);
    __declspec(dllexport) void log_fatal(const char* format, ...);
    */

#ifdef __cplusplus
}
#endif  /* __cplusplus */



#endif // FRAMEWORK_H
