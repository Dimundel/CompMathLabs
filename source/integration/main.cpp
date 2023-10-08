#include "integration.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <vector>

template <typename RealType, unsigned int N>
std::array<RealType, N> linspace(const double start, const double end) {
    std::array<RealType, N> res;
    double delta = (end - start) / (N - 1);
    for (unsigned int i = 0; i < N; ++i) {
        res[i] = (start + i * delta);
    }
    return res;
}

template <typename RealType, unsigned int N>
std::array<double, N> powof10(const std::array<double, N> &x) {
    std::array<double, N> y;
    for (int i = 0; i < x.size(); ++i) {
        y[i] = std::pow(10, x[i]);
    }
    return y;
}

double sin_function(double x) { return std::sin(x); }

int main() {
    std::ofstream data;
    std::array<double, 1000> h = linspace<double, 1000>(-3, 1);
    h = powof10<double, 1000>(h);

    data.open("N3.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16)
             << integrate<decltype(sin_function), double, 3>(sin_function,
             0.0,
                                                             10.0, h[i])
             << std::endl;
    }
    data.close();

    data.open("N5.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16)
             << integrate<decltype(sin_function), double, 5>(sin_function, 0.0,
                                                             10.0, h[i])
             << std::endl;
    }
    data.close();
}