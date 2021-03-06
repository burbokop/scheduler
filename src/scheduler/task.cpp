#include "task.h"

#include <QDebug>
#include <QElapsedTimer>

QString Task::name() const {
    return m_name;
}

size_t Task::arrival() const {
    return m_arrival;
}

void Task::setArrival(size_t arrival) {
    m_arrival = arrival;
}

std::optional<size_t> Task::wcet() const {
    return m_wcet;
}

size_t Task::deadline() const {
    return m_deadline;
}

void Task::setDeadline(size_t deadline) {
    m_deadline = deadline;
}

size_t Task::measure() {
    QElapsedTimer timer;
    timer.start();
    while (!proceed().has_value()) {}
    auto result = timer.elapsed();
    reset();
    return result;
}

void Task::printWcet(Task *task, size_t count) {
    if(task == nullptr || count == 0)
        return;

    size_t min = std::numeric_limits<size_t>::max();
    size_t max = 0;
    size_t sum = 0;
    for (size_t i = 0; i < count; ++i) {
        task->reset();
        auto ms = task->measure();
        if(ms < min) {
            min = ms;
        }
        if(ms > max) {
            max = ms;
        }
        sum += ms;
    }
    qDebug() << "wcet (" << task->name() << "): min:" << min << "aver:" << double(sum) / double(count) << "max:" << max << "ms";
}

void Task::setName(const QString &name)
{
    m_name = name;
}

Task::Task(QString name, std::optional<size_t> wcet, QObject *parent) : QObject(parent), m_name(name), m_wcet(wcet) {}

void Task::autodetectWcet(){
    if(!m_wcet.has_value()) {
        m_wcet = measure();
    }
}

QVariant Task::Result::value() const {
    return m_value;
}

QString Task::Result::name() const {
    return m_name;
}

bool Task::Result::outstanding() const {
    return m_outstanding;
}

Task::Result::Result(QString name, std::optional<QVariant> value):
    m_name(name) {
    if (value.has_value()) {
        m_value = value.value();
        m_outstanding = false;
    } else {
        m_outstanding = true;
    }
}

QDebug &operator <<(QDebug &d, const Task::Result &r) {
    if (r.outstanding()) {
        d << "Task::Result(" << r.name() << "out)";
    } else {
        d << "Task::Result(" << r.name() << ", " << r.value() << ")";
    }
    return d;
}
