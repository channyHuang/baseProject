#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QRunnable>

class WorkThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    void run() override {
        for (int i = 0; i < 1000; ++i) {
            _sleep(10);
        }
        emit sigFinish(index);
    }
signals:
    void sigFinish(int index);

public:
    int index;
};

#endif // WORKTHREAD_H
