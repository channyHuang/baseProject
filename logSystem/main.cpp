#include <QCoreApplication>

#include "framework.h"
#include <thread>

int g_level = 3;

std::string formatlog(const char *format, ...)
{
    std::string str;
    va_list args;
    va_start(args, format);
    {
        int nLength = _vscprintf(format, args);
        nLength += 1;
        std::vector<char> vectorChars(nLength);
        _vsnprintf(vectorChars.data(), nLength, format, args);
        str.assign(vectorChars.data());
    }
    va_end(args);

    //return str;
    writelog(str.c_str(), g_level);
}

void* threadRun(void* args)
{
    for (int i = 0;i < 1e7; ++i)
    {
        formatlog("this is a multithread log writing program %d", i);
    }
}

void cThreadRun()
{
    for (int i = 0; i < 1e7; i++) {
        formatlog("this is a multithread log writing program %d", i);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    init("log", "testLogSystem", 3);
    writelog("xxxxxxxx", 5);
#ifndef WIN32
    pthread_t tids[5];
    for (int i = 0;i < 5; ++i) {
        pthread_create(&tids[i], NULL, threadRun, NULL);
        pthread_join(tids[i], NULL);
    }
#else
    for (int i = 0; i < 5; i++) {
        std::thread cThread(cThreadRun);
        cThread.join();
    }
#endif
    return a.exec();
}
