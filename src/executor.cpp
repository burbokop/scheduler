#include "executor.h"

#include <QDebug>
#include <QElapsedTimer>

#include <src/scheduler/time.h>

void Executor::addTask(Task *task, long deadline) {
    task->reset();
    task->autodetectWcet();
    task->setDeadline(deadline);
    task->setArrival(Time::nowMillis());
    m_tasks.push_back(task);
}

Executor::Executor(QObject *parent) : QObject(parent) {}

Scheduler::ResultVector Executor::exec(Scheduler *scheduler) {
    scheduler->setParent(this);
    scheduler->setTasks(m_tasks);
    scheduler->start();
    while (scheduler->running()) {
        scheduler->proceed();
    }
    return scheduler->results();
}
