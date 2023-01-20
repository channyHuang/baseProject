#include <QCoreApplication>

#include "threadpool.h"

#include <iostream>

void fun(int x) {
    std::cout << x << std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    THREADPOOL::ThreadPool *pool = THREADPOOL::createThreadPool(3, 10);
    for (int i = 0; i < 10; ++i)
        THREADPOOL::addTaskToThreadPool(pool, fun, (void*)i);

    return a.exec();
}
