#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QRegExp>
#include <QTimer>
#include <QFile>
#include <QIODevice>

#include "analyselabels.h"
#include <iostream>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void work(QString qsFileName);

signals:
    void sigHasData(QString qsBuffer);

private:
    AnalyseLabels *m_analyseLabels;
    QThread *m_thread;
};

#endif // MAINWIDGET_H
