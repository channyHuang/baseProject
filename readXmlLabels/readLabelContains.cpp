#include <QtWidgets>
#include <QString>
#include <QRegExp>

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: ~ label inputFileName outputFileName" << std::endl;
        return 0;
    }
    QString qsLabel = QString(argv[1]);


    QFile outFile(argv[3]);
    if (outFile.open(QIODevice::ReadWrite)) {
        QTextStream out(&outFile);

        QFile file(argv[2]);
        if (!file.exists()) {
            std::cout << "File " << argv[2] << " does not exist" << std::endl;
            return 0;
        }
        if(file.open(QIODevice::ReadOnly)) {
            char buffer[1024];
            qint64 lineLen = file.readLine(buffer , sizeof(buffer));
            while (lineLen != -1)
            {
                QString qsBuffer = QString(buffer);
                QStringList qsWordInfo = qsBuffer.split("\t");
                Q_ASSERT(qsWordInfo.size() == 2);
                QString qsWord = qsWordInfo[0];
                QString qsInfo = qsWordInfo[1];

                char sRef[1024];
                sprintf(sRef, "(<%s>.*</%s>)", qsLabel.toStdString().c_str(), qsLabel.toStdString().c_str());
                QRegExp ref(sRef);
                //ref.setMinimal(true);
                int stPos = 0;
                int pos = qsInfo.indexOf(ref, stPos);
                while (pos != -1) {
                    QString qsContains = qsInfo.mid(pos, ref.matchedLength());

                    out << qsWord << "\t" << qsContains << "\n";

                    stPos = pos + ref.matchedLength();
                    pos = qsInfo.indexOf(ref, stPos);
                }


                lineLen = file.readLine(buffer , sizeof(buffer));
            }
        }
        file.close();
    }
    outFile.close();

    return 0;
}
