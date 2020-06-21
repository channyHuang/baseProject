#ifndef ANALYSELABELS_H
#define ANALYSELABELS_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QMap>
#include <QMutexLocker>

extern QMap<QString, QStringList> maps;
extern QMutex mutex;

class AnalyseLabels : public QObject
{
    Q_OBJECT
public:
    explicit AnalyseLabels(QObject *parent = nullptr);

signals:
    void sigHasData();

public slots:
    void analyse(QString qsBuffer);

public:
    int count;
};

#endif // ANALYSELABELS_H
