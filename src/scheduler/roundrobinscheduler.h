#ifndef ROUNDROBINSCHEDULER_H
#define ROUNDROBINSCHEDULER_H

#include "roundindex.h"
#include "scheduler.h"

class RoundRobinScheduler : public Scheduler {
    Q_OBJECT


    RoundIndex<int> currentIndex;
    size_t currentTastStartTime = 0;
    size_t quantum = 0;
public:
    RoundRobinScheduler(size_t quantum, QObject *parent = nullptr);

    // Scheduler interface
public:
    void proceed();
    void start();
};

#endif // ROUNDROBINSCHEDULER_H
