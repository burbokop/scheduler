#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QVector>
#include <random>

class SignalGenerator {
    size_t m_harmonicsCount;
    double m_limitFreq;
    double m_amplitude;
public:
    SignalGenerator(size_t harmonicsCount, double limitFreq, double amplitude = 1);
    double nextDouble();
    double generate(double t);
    QVector<double> generateVec(QVector<double> t);
};

#endif // SIGNALGENERATOR_H
