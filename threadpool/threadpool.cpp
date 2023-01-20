#include "threadpool.h"

namespace THREADPOOL {

ThreadPool::ThreadPool(int threadCount, int taskCount)
    : maxThreadCount(threadCount), maxTaskCount(taskCount)
{
    while(!tasks.empty()) {
        tasks.pop();
    }
    semaphore.release(maxThreadCount);
}

ThreadPool::~ThreadPool() {}

void threadPoolSingleTaskThread(ThreadPool* threadPool, ThreadTask task) {
    // 执行具体任务
    //threadPool->semaphore.acquire();
    std::thread singleTaskThread(task.func, task.arg);
    singleTaskThread.join();
    threadPool->semaphore.release();
    // callback or get thread result
}

void threadPoolTasksThread(ThreadPool* threadPool) {
    while (true) {
        threadPool->semaphore.acquire();

        std::unique_lock lock(threadPool->mutex);
        while (threadPool->tasks.empty()) {
            threadPool->cond.wait(lock);
        }

        ThreadTask task = threadPool->tasks.front();
        threadPool->tasks.pop();

        lock.unlock();

        std::thread taskThread(threadPoolSingleTaskThread, threadPool, task);
        taskThread.detach();
    }
}

ThreadPool* createThreadPool(int threadCount, int taskCount) {
    try {
        ThreadPool* threadPool = new ThreadPool(threadCount, taskCount);

        std::thread taskThread(threadPoolTasksThread, threadPool);
        taskThread.detach();

        sleep(1);
        return threadPool;
    } catch (...) {
        // new error
    }
}

bool addTaskToThreadPool(ThreadPool* threadPool, void* (*func)(void* arg), void* arg) {
    std::unique_lock lock(threadPool->mutex);
    if (threadPool->taskQueueSize >= threadPool->maxTaskCount) {
        return false;
    }
    threadPool->tasks.push(ThreadTask(func, arg));
    return true;
}

void destroyThreadPool(ThreadPool* threadPool) {

}

}
