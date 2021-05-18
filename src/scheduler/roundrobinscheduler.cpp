#include "roundrobinscheduler.h"
#include "time.h"
#include <QtDebug>
RoundRobinScheduler::RoundRobinScheduler(size_t quantum, QObject *parent) : Scheduler(parent) {
    this->quantum = quantum;
}

void RoundRobinScheduler::proceed() {
    currentIndex.setLimit(taskCount());
    if (hasResult(currentIndex)) {
        currentIndex++;
    }
    auto now = Time::nowMillis();
    if (now - currentTastStartTime < quantum) {
        if (proceedTask(currentIndex)) {
            currentIndex++;
            currentTastStartTime = now;
        }
    } else {
        currentIndex++;
        currentTastStartTime = now;
    }
}

void RoundRobinScheduler::start() {
    currentTastStartTime = Time::nowMillis();
}
