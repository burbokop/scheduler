#ifndef FOURIER_H
#define FOURIER_H

#include <functional>
#include <complex>

struct Fourier {
    static std::function<double(size_t)> transform(std::function<double(size_t)> x, size_t N);
};

#endif // FOURIER_H
