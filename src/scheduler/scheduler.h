#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

#include <QVariant>

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
    class LogValue {
        size_t m_time;
        QString m_name;
    public:
        LogValue(size_t time, QString name);
        QString name() const;
        size_t time() const;
    };
private:
    QList<LogValue> m_log;
public:

    Task* task(int i) const;
    int taskCount() const;
    bool proceedTask(int taskIndex);
    bool hasResult(int i) const;
    Scheduler(QObject *parent = nullptr);
    virtual void proceed() = 0;
    virtual void start() = 0;
    bool running() const;
    void setTasks(const QVector<Task *> &tasks);
    ResultVector results() const;
    QList<LogValue> log() const;

    static QList<LogValue> removeRepeatedNames(QList<LogValue> log);
};

#endif // SCHEDULER_H
