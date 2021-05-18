#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QObject>
#include "scheduler/scheduler.h"

class Executor : public QObject {
    Q_OBJECT
    QVector<Task*> m_tasks;
public:
    void addTask(Task* task, long deadline);
    explicit Executor(QObject *parent = nullptr);

    Scheduler::ResultVector exec(Scheduler *scheduler);

signals:

};

#endif // EXECUTOR_H
