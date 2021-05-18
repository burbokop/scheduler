#ifndef ERLIESTDEADLINEFIRSTSCHEDULER_H
#define ERLIESTDEADLINEFIRSTSCHEDULER_H

#include "scheduler.h"

class ErliestDeadlineFirstScheduler : public Scheduler {
    Q_OBJECT
public:
    ErliestDeadlineFirstScheduler(QObject *parent = nullptr);

    // Scheduler interface
public:
    void proceed();
    void start();
};

#endif // ERLIESTDEADLINEFIRSTSCHEDULER_H
