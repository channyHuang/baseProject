#include "analyselabels.h"
#include <iostream>

AnalyseLabels::AnalyseLabels(QObject *parent) : QObject(parent)
{
    count = 0;
}

void AnalyseLabels::analyse(QString qsBuffer) {
    std::cout << __FUNCTION__ << " " << QThread::currentThreadId() << std::endl;
    QStringList qsWordInfo = qsBuffer.split("\t");
    if (qsWordInfo.size() != 2) {
        count++;
        return;
    }
    QString qsWord = qsWordInfo[0];
    QString qsInfo = qsWordInfo[1];

    QRegExp ref("(<[a-zA-Z]+>|<[a-zA-Z]+ .*>)");
    ref.setMinimal(true);
    int stPos = 0;
    int pos = qsInfo.indexOf(ref, stPos);
    while (pos != -1) {
        QString qsLabel = qsInfo.mid(pos + 1, ref.matchedLength() - 2);
        int nSpace = qsLabel.indexOf(' ');
        qsLabel = qsLabel.mid(0, nSpace);

        QMutexLocker locker(&mutex);
        QMap<QString, QStringList>::iterator itr = maps.find(qsLabel);
        if (itr == maps.end()) {
            maps[qsLabel] = (QStringList() << qsWord);
        } else if (itr.value().size() < 3 && !itr.value().contains(qsWord)){
            itr.value() << qsWord;
        }
        locker.unlock();

        stPos = pos + ref.matchedLength();
        pos = qsInfo.indexOf(ref, stPos);
    }
}
