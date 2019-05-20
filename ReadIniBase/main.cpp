#include <QCoreApplication>

#include "readini.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ReadIni iniReader;
    iniReader.setIniName("./test.ini");
    QString value = iniReader.getValue("key", "");
    qDebug() << value;

    return a.exec();
}
