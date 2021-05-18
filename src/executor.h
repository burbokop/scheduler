#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QObject>
#include "scheduler/scheduler.h"

class Executor : public QObject {
    Q_OBJECT
    K_QML_TYPE(Executor)
    K_READONLY_PROPERTY(SchedulerLog, log, log, setLog, logChanged, SchedulerLog())
    QVector<Task*> m_tasks;
public:
    void addTask(Task* task, size_t deadline);
    explicit Executor(QObject *parent = nullptr);

    Scheduler::ResultVector exec(Scheduler *scheduler);

signals:

};

#endif // EXECUTOR_H
