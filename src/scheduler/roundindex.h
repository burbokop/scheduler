#ifndef ROUNDINDEX_H
#define ROUNDINDEX_H

#include <numeric>

template<typename T>
class RoundIndex {
    T index = 0;
    T limit;
    bool hasLimit = true;
public:
    RoundIndex() { this->hasLimit = false; }
    RoundIndex(T limit) { this->limit = limit; }
    void setLimit(T limit) {
        this->limit = limit;
        hasLimit = true;
    }
    void increment() {
        if (++index >= limit && hasLimit) {
            index = 0;
        }
    }
    RoundIndex operator++(int) {
        auto cp = *this;
        increment();
        return cp;
    }
    RoundIndex operator++() {
        increment();
        return *this;
    }
    operator T() const { return index; }
};

#endif // ROUNDINDEX_H
