#include <QCoreApplication>

#include <iostream>
using namespace std;

#include "NetWorkManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    NetWorkManager *manager = new NetWorkManager();
    manager->init();

    return app.exec();
}
