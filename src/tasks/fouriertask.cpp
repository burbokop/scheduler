#include "fouriertask.h"

#include <src/tools/fourier.h>
#include <src/tools/range.h>
#include <src/tools/signalgenerator.h>
#include <QDebug>

FourierTask::FourierTask(QString name, std::optional<long> wcet, QObject *parent) : Task(name, wcet, parent) {}

std::optional<QVariant> FourierTask::proceed() {
    if (index < len) {
        result.push_back(fourierFunc(index++));
        return std::nullopt;
    } else {
        return QVariant::fromValue(result);
    }
}

void FourierTask::reset() {
    SignalGenerator signalGenerator(10, 900);
    y = signalGenerator.generateVec(Range::gen<double>(0, 256));
    fourierFunc = Fourier::transform([this](auto x){ return y[x]; }, y.length());
    len = y.length();
    result.clear();
    result.reserve(len);
    index = 0;
}
