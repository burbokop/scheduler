#include "scheduler.h"
#include "time.h"

bool Scheduler::running() const {
    if (m_results.length() < m_tasks.length()) {
        return true;
    }
    bool hasVal = true;
    for(const auto& r : m_results) {
        if (!r.has_value()) {
            hasVal = false;
            break;
        }
    }
    return !hasVal;
}

void Scheduler::setTasks(const QVector<Task *> &tasks) {
    m_tasks = tasks;
}

void Scheduler::addTask(Task *task) {
    m_tasks.push_back(task);
}

Task *Scheduler::task(int i) const {
    if (i < m_tasks.length()) {
        return m_tasks[i];
    } else {
        return nullptr;
    }
}

int Scheduler::taskCount() const {
    return m_tasks.length();
}

bool Scheduler::proceedTask(int taskIndex) {
    auto t = task(taskIndex);
    if (t) {
        auto time = Time::nowMillis();
        if(time > t->deadline()) {
            markResultOutstanding(taskIndex, t->name());
            return true;
        } else {
            auto result = t->proceed();
            m_log.push_back(SchedulerLogValue(time, time, t->name()));
            if (result.has_value()) {
                setResult(taskIndex, t->name(), result.value());
                return true;
            }
        }
    }
    return false;
}

Scheduler::ResultVector Scheduler::results() const {
    return m_results;
}

void Scheduler::setResult(int i, QString name, QVariant value) {
    if (i >= m_results.size()) {
        m_results.resize(i + 1);
    }
    m_results[i] = Task::Result(name, value);
}

void Scheduler::markResultOutstanding(int i, QString name) {
    if (i >= m_results.size()) {
        m_results.resize(i + 1);
    }
    m_results[i] = Task::Result(name, std::nullopt);
}

SchedulerLog Scheduler::log() const {
    return m_log;
}

SchedulerLog Scheduler::removeRepeatedNames(SchedulerLog log) {
    auto it = log.begin();
    while (it != log.end()) {
        if(it != log.begin() && it->name() == (it - 1)->name()) {
            *(it - 1) = SchedulerLogValue((it - 1)->startTime(), it->endTime(), it->name());
            it = log.erase(it);
        } else {
            it++;
        }
    }
    return log;
}

bool Scheduler::hasResult(int i) const {
    if (i < m_results.size()) {
        return m_results[i].has_value();
    }
    return false;
}

Scheduler::Scheduler(QObject *parent) : QObject(parent) {}

void Scheduler::reset() {
    m_log.clear();
    m_results.clear();
}

QString SchedulerLogValue::name() const {
    return m_name;
}

size_t SchedulerLogValue::startTime() const {
    return m_startTime;
}

size_t SchedulerLogValue::endTime() const {
    return m_endTime;
}

SchedulerLogValue::SchedulerLogValue(size_t startTime, size_t endTime, QString name):
    m_startTime(startTime), m_endTime(endTime), m_name(name) {}

bool operator==(const SchedulerLogValue &val0, const SchedulerLogValue &val1) {
    return val0.name() == val1.name() && val0.startTime() == val1.startTime() && val0.endTime() == val1.endTime();
}

QDebug &operator <<(QDebug &d, const SchedulerLogValue &v) {
    return d << "(" << v.name() << ", " << v.startTime() << ", " << v.endTime() << ")";
}
