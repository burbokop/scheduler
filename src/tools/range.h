#ifndef RANGE_H
#define RANGE_H

#include <QVector>



struct Range {
    template<typename T>
    static QVector<T> gen(size_t from, size_t until) {
        QVector<T> result;
        result.reserve(until - from);
        for(size_t i = from; i < until; ++i) {
            result.push_back(i);
        }
        return result;
    }
};

#endif // RANGE_H
