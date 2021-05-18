#include "autocorrelationtask.h"

#include <src/tools/correlation.h>
#include <src/tools/range.h>
#include <src/tools/signalgenerator.h>

AutoCorrelationTask::AutoCorrelationTask(QString name, std::optional<long> wcet, QObject *parent) : Task(name, wcet, parent) {}

std::optional<QVariant> AutoCorrelationTask::proceed() {
    if (it.notCompleated()) {
        it++;
        return std::nullopt;
    } else {
        return QVariant::fromValue(it.result());
    }
}

void AutoCorrelationTask::reset() {
    SignalGenerator signalGenerator(10, 900);
    it = Correlation::iterator(signalGenerator.generateVec(Range::gen<double>(0, 512)));
}
