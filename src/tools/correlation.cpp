#include "correlation.h"


double Correlation::covariant(const QVector<double> &seq0, const QVector<double> &seq1) {
    if (seq0.length() == seq1.length() && seq0.length() > 1) {
        auto aver0 = std::accumulate(seq0.begin(), seq0.end(), 0) / seq0.length();
        auto aver1 = std::accumulate(seq1.begin(), seq1.end(), 0) / seq1.length();
        double sum = 0;
        for (auto i = 0; i < seq0.length(); ++i) {
            sum += (seq0[i] - aver0) * (seq1[i] - aver1);
        }
        return sum;
    }
    return 0;
}

QVector<double> Correlation::apply(const QVector<double> &seq0, const QVector<double> &seq1) {
    if (seq0.length() == seq1.length() && seq0.length() > 1) {
        auto halfSize = std::round(seq0.length() / 2);
        QVector<double> result;
        result.reserve(halfSize);
        for (auto i = 0; i < halfSize; ++i) {
            result.push_back(covariant(seq0.mid(0, halfSize), seq1.mid(i, halfSize)) / (seq0.length() - 1));
        }
        return result;
    }
    return QVector<double>();
}

Correlation::Iterator Correlation::iterator(const QVector<double> &seq0, const QVector<double> &seq1) {
    if (seq0.length() == seq1.length() && seq0.length() > 1) {
        Iterator it;
        it.seq0 = seq0;
        it.seq1 = seq1;
        it.halfSize = std::round(seq0.length() / 2);
        it.m_result.reserve(it.halfSize);
        return it;
    }
    return Iterator();
}

bool Correlation::Iterator::operator++(int) {
    if(index < halfSize) {
        m_result.push_back(covariant(seq0.mid(0, halfSize), seq1.mid(index, halfSize)) / (seq0.length() - 1));
        index++;
        return false;
    }
    return true;
}
