#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QThreadPool>

#include <vector>
using namespace std;

#include "workthread.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void sltFinish(int index);
    void startThread();

private:
    QPushButton *btn = nullptr;
    vector<WorkThread> threads;
};
#endif // WIDGET_H
