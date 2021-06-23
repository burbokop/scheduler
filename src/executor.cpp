#include "executor.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QThread>
#include <queue>

#include <src/scheduler/time.h>
#include <execution>
#include <chrono>
#include <random>

void Executor::addTask(Task *task, size_t deadline) {
    task->reset();
    task->autodetectWcet();
    task->setDeadline(deadline);
    task->setArrival(Time::nowMillis());
    m_tasks.push_back(task);
}

void Executor::addErlangFlow(const ErlangFlow &flow) {
    m_erlangFlows.push_back(flow);
}

Executor::Executor(QObject *parent) : QObject(parent) {}

Scheduler::ResultVector Executor::exec(Scheduler *scheduler) {
    scheduler->setParent(this);
    scheduler->setTasks(m_tasks);
    scheduler->reset();
    scheduler->start();
    while (scheduler->running()) {
        scheduler->proceed();
    }
    setLog(QList<SchedulerLog> { Scheduler::removeRepeatedNames(scheduler->log()) });
    return scheduler->results();
}

void Executor::execConcurent(const QVector<Scheduler *> &processors, QVector<ErlangFlow> flows, size_t duration) {
    using namespace std::chrono_literals;

    std::vector<std::queue<Task*>> m_queues;
    m_queues.resize(processors.size());

    std::promise<bool> flowPromise;
    auto flowFuture = flowPromise.get_future();
    std::thread flowThread([&flows, &processors, duration, &m_queues, &flowPromise](){
        std::default_random_engine eng(static_cast<long unsigned int>(time(0)));
        std::uniform_int_distribution<size_t> uid(0, processors.size() - 1);

        QElapsedTimer mainTimer;
        mainTimer.start();
        for (auto &flow : flows) {
            flow.resetTimer();
        }

        while (mainTimer.elapsed() < qint64(duration)) {
            for (auto &flow : flows) {
                if(flow.checkTimer()) {
                    auto task = flow.factory()();
                    if(task) {                        
                        task->reset();
                        task->setName(flow.name() + QString::number(flow.index()));
                        task->setDeadline(Time::nowMillis() + flow.deadlineInterval());
                        task->setArrival(Time::nowMillis());
                        m_queues[uid(eng)].push(task);
                        flow.incrementIndex();
                    }
                }
            }
        }
        flowPromise.set_value(true);
    });

    std::atomic<size_t> index;
    std::for_each(std::execution::par_unseq, processors.begin(), processors.end(), [&flowFuture, &index, &m_queues](Scheduler* processor){
        const auto currentIndex = index++;
        auto& queue = m_queues[currentIndex];
        QElapsedTimer t;
        t.restart();
        while (flowFuture.wait_for(0ms) != std::future_status::ready || processor->running()) {
            while (!queue.empty()) {
                processor->addTask(queue.front());
                queue.pop();
            }

            if(processor->running()) {
                processor->proceed();
            }
        }
    });
    flowThread.join();
}

size_t ErlangFlow::period() const {
    return m_period;
}

std::function<Task *()> ErlangFlow::factory() const {
    return m_factory;
}

bool ErlangFlow::checkTimer() {
    if (m_timer.elapsed() >= qint64(m_period)) {
        m_timer.restart();
        return true;
    }
    return false;
}

void ErlangFlow::resetTimer() {
    m_timer.restart();
}

QString ErlangFlow::name() const {
    return m_name;
}

size_t ErlangFlow::deadlineInterval() const {
    return m_deadlineInterval;
}

void ErlangFlow::incrementIndex() {
    m_index++;
}

size_t ErlangFlow::index() const {
    return m_index;
}

ErlangFlow::ErlangFlow(const QString &name, size_t period, size_t deadlineInterval, const std::function<Task *()> &factory) {
    m_name = name;
    m_period = period;
    m_deadlineInterval = deadlineInterval;
    m_factory = factory;
}

ErlangFlow ErlangFlow::fromIntencity(const QString &name, double intencity, size_t deadlineInterval, const std::function<Task *()> &factory) {
    return ErlangFlow(name, 1000 / intencity, deadlineInterval, factory);
}

