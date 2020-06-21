#include <QtWidgets>

#include <iostream>
#include "mainwidget.h"

QMap<QString, QStringList> maps;
QMutex mutex;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWidget widget;
    widget.work("test.txt");

    std::cout << "start to write " << maps.size() << std::endl;
    QFile file;
    file.setFileName("allLabels.txt");
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream out(&file);
        for (QMap<QString, QStringList>::iterator itr = maps.begin(); itr != maps.end(); itr++) {
            out << itr.key() << "\t";
            for (QString qsWord : itr.value()) {
                out << qsWord << " ";
            }
            out << "\n";
        }
    }
    file.close();

    return app.exec();
}
