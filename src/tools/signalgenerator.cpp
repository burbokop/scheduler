#include "signalgenerator.h"

SignalGenerator::SignalGenerator(size_t harmonicsCount, double limitFreq, double amplitude) {
    m_harmonicsCount = harmonicsCount;
    m_limitFreq = limitFreq;
    m_amplitude = amplitude;
    std::srand(clock());
}

double SignalGenerator::nextDouble() { return double(std::rand()) / RAND_MAX; }

double SignalGenerator::generate(double t) {
    if(m_harmonicsCount != 0) {
        auto delta = m_limitFreq / m_harmonicsCount;
        auto amplitudeRandom = nextDouble();
        auto phase = nextDouble();
        double sum = 0;
        for (size_t i = 0; i < m_harmonicsCount; ++i) {
            sum += m_amplitude * amplitudeRandom * std::sin((i + 1) * delta * t + phase);
        }
        return sum;
    }
    return 0.;
}

QVector<double> SignalGenerator::generateVec(QVector<double> t) {
    QVector<double> result;
    result.reserve(t.length());
    for(auto ct : t) {
        result.push_back(generate(ct));
    }
    return result;
}
