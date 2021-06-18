#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QObject>
#include <src/kadditional.h>
#include "scheduler/scheduler.h"

class Executor : public QObject {
    Q_OBJECT
    K_QML_TYPE(Executor)
    K_READONLY_PROPERTY(QList<SchedulerLog>, log, log, setLog, logChanged, QList<SchedulerLog>())
    QVector<Task*> m_tasks;

public:
    void addTask(Task* task, size_t deadline);
    explicit Executor(QObject *parent = nullptr);

    Scheduler::ResultVector exec(Scheduler *scheduler);

    template<typename SchedulerType, typename ...Args>
    Scheduler::ResultVector exec(size_t processorsCount, Args... args) {
        if(processorsCount == 0)
            return {};

        QVector<Scheduler*> processors;
        processors.reserve(processorsCount);
        const auto taskGroups = KAdditional::groupedOn(m_tasks, processorsCount);

        for(size_t i = 0; i < processorsCount; ++i) {
            auto processor = new SchedulerType(args...);
            processor->setParent(this);
            processor->setTasks(taskGroups[i]);
            processor->reset();
            processor->start();
            processors.push_back(processor);
        }

        auto isAnyRunning = [processors]() -> bool {
            for(const auto& p : processors) {
                if (p->running()) {
                    return true;
                }
            }
            return false;
        };

        while (isAnyRunning()) {
            for(auto processor : processors) {
                if(processor->running()) {
                    processor->proceed();
                }
            }
        }

        QList<SchedulerLog> log;
        Scheduler::ResultVector result;
        log.reserve(processors.size());
        result.reserve(processors.size());
        for(const auto& processor : processors) {
            log.push_back(Scheduler::removeRepeatedNames(processor->log()));
            result += processor->results();
        }
        setLog(log);
        return result;
    }

signals:

};

#endif // EXECUTOR_H
