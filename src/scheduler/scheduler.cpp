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
            m_log.push_back(LogValue(time, t->name()));
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

QList<Scheduler::LogValue> Scheduler::log() const {
    return m_log;
}

QList<Scheduler::LogValue> Scheduler::removeRepeatedNames(QList<Scheduler::LogValue> log) {
    auto it = log.begin();
    while (it != log.end()) {
        if(it != log.begin() && it->name() == (it - 1)->name()) {
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

size_t Scheduler::LogValue::time() const {
    return m_time;
}

QString Scheduler::LogValue::name() const {
    return m_name;
}

Scheduler::LogValue::LogValue(size_t time, QString name):
    m_time(time), m_name(name) {}
