#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

#include <QVariant>
#include <klibcore/src/kmacro.h>

class SchedulerLogValue {
    size_t m_startTime;
    size_t m_endTime;
    QString m_name;
public:
    SchedulerLogValue(size_t startTime = 0, size_t endTime = 0, QString name = QString());
    QString name() const;
    size_t startTime() const;
    size_t endTime() const;
};
typedef QList<SchedulerLogValue> SchedulerLog;

bool operator==(const SchedulerLogValue& val0, const SchedulerLogValue& val1);
QDebug &operator << (QDebug &d, const SchedulerLogValue& v);

Q_DECLARE_METATYPE(SchedulerLogValue)
Q_DECLARE_METATYPE(SchedulerLog)
class SchedulerLogValueMeta {
    K_META_TYPE(SchedulerLogValue)
    K_META_TYPE(SchedulerLog)
};

class Scheduler : public QObject {
    Q_OBJECT
    QVector<Task*> m_tasks;
public:
    typedef QVector<std::optional<Task::Result>> ResultVector;
private:
    ResultVector m_results;
    void setResult(int i, QString name, QVariant value);
    void markResultOutstanding(int i, QString name);
public:

private:
    SchedulerLog m_log;
public:

    Task* task(int i) const;
    int taskCount() const;
    bool proceedTask(int taskIndex);
    bool hasResult(int i) const;
    Scheduler(QObject *parent = nullptr);
    virtual void proceed() = 0;
    void reset();
    virtual void start() = 0;
    bool running() const;
    void setTasks(const QVector<Task *> &tasks);
    ResultVector results() const;
    SchedulerLog log() const;

    static SchedulerLog removeRepeatedNames(SchedulerLog log);
};


#endif // SCHEDULER_H
