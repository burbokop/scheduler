#include "fourier.h"


std::function<double (size_t)> Fourier::transform(std::function<double (size_t)> x, size_t N) {
    return [=](size_t m) -> double {
        std::complex<double> sum;
        for(size_t n = 0; n < N; ++n) {
            sum += x(n) * std::complex<double>(std::cos(2 * M_PI * n * m / N), -std::sin(2 * M_PI * n * m / N));
        }
        return std::abs(sum);
    };
}
