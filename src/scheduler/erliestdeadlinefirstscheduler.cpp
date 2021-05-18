#include "erliestdeadlinefirstscheduler.h"
#include "time.h"
#include <QtDebug>
ErliestDeadlineFirstScheduler::ErliestDeadlineFirstScheduler(QObject *parent) : Scheduler(parent) {

}

void ErliestDeadlineFirstScheduler::proceed() {
    size_t minDelta = std::numeric_limits<size_t>::max();
    size_t minDeltaTaskIndex = 0;
    for(auto i = 0; i < taskCount(); ++i) {
        auto currentTask = task(i);
        if (!hasResult(i) && currentTask) {
             auto delta = currentTask->deadline() - Time::nowMillis();
             if (delta < minDelta) {
                 minDelta = delta;
                 minDeltaTaskIndex = i;
             }
        }
    }
    proceedTask(minDeltaTaskIndex);
}

void ErliestDeadlineFirstScheduler::start() {}
