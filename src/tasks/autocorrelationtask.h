#ifndef AUTOCORRELATIONTASK_H
#define AUTOCORRELATIONTASK_H

#include <src/scheduler/task.h>

#include <src/tools/correlation.h>

class AutoCorrelationTask : public Task {
    Q_OBJECT
    Correlation::Iterator it;
public:
    AutoCorrelationTask(QString name, std::optional<long> wcet = std::nullopt, QObject *parent = nullptr);

    // Task interface
public:
    std::optional<QVariant> proceed();
    void reset();
};

#endif // AUTOCORRELATIONTASK_H
