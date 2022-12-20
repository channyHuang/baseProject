#include "widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    btn = new QPushButton(this);
    btn->setText("push!");
    connect(btn, &QPushButton::clicked, this, &Widget::startThread);
}

Widget::~Widget()
{
    QThreadPool::globalInstance()->waitForDone();
}

void Widget::startThread() {
    QThreadPool::globalInstance()->setMaxThreadCount(3);
    vector<WorkThread> tmpthreads(100);
    threads.swap(tmpthreads);
    for (int i = 0; i < 100; ++i) {
        threads[i].index = i;
        threads[i].setAutoDelete(false);
        connect(&threads[i], &WorkThread::sigFinish, this, &Widget::sltFinish);
        QThreadPool::globalInstance()->start(&threads[i]);
    }
}

void Widget::sltFinish(int index) {
    qDebug() << index << "finish";
}
