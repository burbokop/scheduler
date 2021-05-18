#ifndef FOURIERTASK_H
#define FOURIERTASK_H

#include "../scheduler/task.h"

class FourierTask : public Task {
    Q_OBJECT
    size_t index = 0;
    size_t len = 0;
    std::function<double(size_t)> fourierFunc;
    QVector<double> result;
    QVector<double> y;
public:
    FourierTask(QString name, std::optional<long> wcet = std::nullopt, QObject *parent = nullptr);

    // Task interface
public:
    std::optional<QVariant> proceed();
    void reset();
};

#endif // FOURIERTASK_H
