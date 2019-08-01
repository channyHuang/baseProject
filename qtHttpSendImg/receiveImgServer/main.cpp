#include <QCoreApplication>

#include "atcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ATcpServer server;

    return a.exec();
}
