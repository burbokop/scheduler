#ifndef CORRELATION_H
#define CORRELATION_H

#include <QVector>
#include <algorithm>
#include <math.h>

struct Correlation {
    static double covariant(const QVector<double> &seq0, const QVector<double> &seq1);

    static QVector<double> apply(const QVector<double> &seq0, const QVector<double> &seq1);

    static inline QVector<double> apply(const QVector<double> &seq) { return apply(seq, seq); }

    class Iterator {
        friend Correlation;
        QVector<double> seq0;
        QVector<double> seq1;
        size_t halfSize;
        QVector<double> m_result;
        size_t index = 0;
    public:
        bool notCompleated() { return index < halfSize; }
        bool operator++(int);
        QVector<double> result() const { return m_result; }
    };

    static Iterator iterator(const QVector<double> &seq0, const QVector<double> &seq1);
    static inline Iterator iterator(const QVector<double> &seq) { return iterator(seq, seq); }

};

#endif // CORRELATION_H
