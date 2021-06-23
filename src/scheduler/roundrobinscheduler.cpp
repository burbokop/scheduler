#include "roundrobinscheduler.h"
#include "time.h"
#include <QtDebug>
RoundRobinScheduler::RoundRobinScheduler(size_t quantum, QObject *parent) : Scheduler(parent) {
    this->quantum = quantum;
}

void RoundRobinScheduler::proceed() {
    currentIndex.setLimit(taskCount());
    for (int i = 0; i < taskCount() && hasResult(currentIndex); ++i) {
        currentIndex++;
    }

    if(hasResult(currentIndex)) {
        return;
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
