#include <QCoreApplication>

#include "dllmanager.h"
#include <iostream>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    DllManager *manager = new DllManager();
    bool bLoad = manager->loadDll();
    manager->test();

    //return a.exec();
    return 0;
}
