#include <QCoreApplication>


#include "dllmanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DllManager manager;
    manager.loadDllClass();

    return a.exec();
}
