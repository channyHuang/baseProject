#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <functional>
#include <thread>       // c++11
#include <condition_variable>
#include <semaphore>    // c++20
#ifdef WIN32_T
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace THREADPOOL {

const int MAX_TASK_COUNT = 1000;

struct ThreadTask
{
    void* (*func)(void *arg);
    void* arg;

    ThreadTask(void* (*fun)(void* arg), void* a) {
        func = fun;
        arg = a;
    }
};

class ThreadPool
{
public:
    ThreadPool(int threadCount, int taskCount);
    ~ThreadPool();

    std::mutex mutex;
    std::condition_variable cond;       // 等待任务队列不为空
    std::counting_semaphore<MAX_TASK_COUNT> semaphore{0};  // 控制最大运行的线程数

    std::queue<ThreadTask> tasks;

    int maxThreadCount;                 //最大运行的线程数
    int maxTaskCount;                   //最大的任务数

    int taskQueueSize;
};

ThreadPool* createThreadPool(int threadCount = 1, int taskCount = MAX_TASK_COUNT);
bool addTaskToThreadPool(ThreadPool* pool, void* (*func)(void* arg), void* arg);
void destroyThreadPool(ThreadPool* threadPool);

}

#endif // THREADPOOL_H
