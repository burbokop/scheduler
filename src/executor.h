#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QElapsedTimer>
#include <QObject>
#include <src/kadditional.h>
#include "scheduler/scheduler.h"

class ErlangFlow {
    size_t m_period = 0;
    size_t m_deadlineInterval = 0;
    std::function<Task*()> m_factory;
    size_t m_index = 0;
    QElapsedTimer m_timer;
    QString m_name;
public:
    ErlangFlow(const QString& name, size_t period, size_t deadlineInterval, const std::function<Task*()>& factory);
    static ErlangFlow fromIntencity(const QString& name, double intencity, size_t deadlineInterval, const std::function<Task*()>& factory);
    size_t period() const;
    std::function<Task *()> factory() const;
    bool checkTimer();
    void resetTimer();
    QString name() const;
    size_t deadlineInterval() const;
    void incrementIndex();
    size_t index() const;
};

class Executor : public QObject {
    Q_OBJECT
    K_QML_TYPE(Executor)
    K_READONLY_PROPERTY(QList<SchedulerLog>, log, log, setLog, logChanged, QList<SchedulerLog>())
    QVector<Task*> m_tasks;
    QVector<ErlangFlow> m_erlangFlows;
public:
    void addTask(Task* task, size_t deadline);
    void addErlangFlow(const ErlangFlow& flow);
    explicit Executor(QObject *parent = nullptr);

    Scheduler::ResultVector exec(Scheduler *scheduler);

    static void execConcurent(const QVector<Scheduler*>& processors, QVector<ErlangFlow> flows, size_t duration);

    template<typename SchedulerType, typename ...Args>
    Scheduler::ResultVector exec(size_t processorsCount, size_t duration, Args... args) {
        if(processorsCount == 0)
            return {};

        QVector<Scheduler*> processors;
        processors.reserve(processorsCount);
        const auto taskGroups = KAdditional::groupedOn(m_tasks, processorsCount);

        for(size_t i = 0; i < processorsCount; ++i) {
            auto processor = new SchedulerType(args...);
            processor->setParent(this);
            if(int(i) < taskGroups.size()) {
                processor->setTasks(taskGroups[i]);
            }
            processor->reset();
            processor->start();
            processors.push_back(processor);
        }

        execConcurent(processors, m_erlangFlows, duration);

        QList<SchedulerLog> log;
        Scheduler::ResultVector result;
        log.reserve(processors.size());
        result.reserve(processors.size());
        for(const auto& processor : processors) {
            auto ll = Scheduler::removeRepeatedNames(processor->log());
            qDebug() << "log size:" << processor->log().size() << ll.size();
            log.push_back(ll);
            result += processor->results();
        }
        setLog(log);
        return result;
    }

signals:

};

#endif // EXECUTOR_H
