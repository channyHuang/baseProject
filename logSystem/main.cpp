#include <QCoreApplication>

#include "logThread.h"
#include "logManager.h"

#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    LogManager *s = LogManager::getInstance();

    s->writeLog("test");

    return a.exec();
}
