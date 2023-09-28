#include "interpolation.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
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
std::array<RealType, N> chebishev(const double start, const double end) {
    std::array<RealType, N> res;
    for (int i = N - 1; i >= 0; --i) {
        res[N - 1 - i] =
            (end + start) / 2 +
            (end - start) * (std::cos((M_PI * (2 * i + 1)) / (2 * N))) / 2;
    }
    return res;
}

template <typename RealType, unsigned int N>
std::array<double, N> exp(const std::array<double, N> &x) {
    std::array<double, N> y;
    for (int i = 0; i < x.size(); ++i) {
        y[i] = std::exp(x[i]);
    }
    return y;
}

template <typename RealType, unsigned int N>
std::array<double, N> powof2(const std::array<double, N> &x) {
    std::array<double, N> y;
    for (int i = 0; i < x.size(); ++i) {
        y[i] = std::pow(2, x[i]);
    }
    return y;
}

template <typename RealType, unsigned int N>
std::array<double, N> powof10(const std::array<double, N> &x) {
    std::array<double, N> y;
    for (int i = 0; i < x.size(); ++i) {
        y[i] = std::pow(10, x[i]);
    }
    return y;
}

const unsigned int NUM_OF_DOTS = 1000;
const unsigned int NUM_OF_LENGTHS = 1000;

int main() {
    std::ofstream data;
    // data.open("data3chebishev.txt");
    // std::array<double, NUM_OF_LENGTHS> length_array =
    //     powof10<double, NUM_OF_LENGTHS>(
    //         linspace<double, NUM_OF_LENGTHS>(-7, 1));
    // for (unsigned int i = 1; i < NUM_OF_LENGTHS; ++i) {
    //     std::array<double, 3> points3 =
    //         chebishev<double, 3>(0, length_array[i]);
    //     std::array<double, 3> points3y = exp<double, 3>(points3);
    //     NewtonInterpolator<double, double, 3> interpolator3(points3,
    //     points3y); std::array<double, NUM_OF_DOTS> x =
    //         linspace<double, NUM_OF_DOTS>(0, length_array[i]);
    //     for (int j = 0; j < NUM_OF_DOTS; ++j) {
    //         data << std::setprecision(16) << interpolator3.interpolate(x[j])
    //              << std::endl;
    //     }
    // }
    // data.close();

    // data.open("data4chebishev.txt");
    // std::array<double, 1000> length_array =
    //     powof10<double, 1000>(linspace<double, 1000>(-7, 1));
    // for (unsigned int i = 1; i < 1000; ++i) {
    //     std::array<double, 4> points4 =
    //         chebishev<double, 4>(0, length_array[i]);
    //     std::array<double, 4> points4y = exp<double, 4>(points4);
    //     NewtonInterpolator<double, double, 4> interpolator4(points4,
    //     points4y); std::array<double, 1000> x = linspace<double, 1000>(0,
    //     length_array[i]); for (int j = 0; j < 1000; ++j) {
    //         data << std::setprecision(16) << interpolator4.interpolate(x[j])
    //              << std::endl;
    //     }
    // }
    // data.close();

    data.open("data5chebishev.txt");
    std::array<double, 1000> length_array =
        powof10<double, 1000>(linspace<double, 1000>(-7, 1));
    for (unsigned int i = 1; i < 1000; ++i) {
        std::array<double, 5> points5 =
            chebishev<double, 5>(0, length_array[i]);
        std::array<double, 5> points5y = exp<double, 5>(points5);
        NewtonInterpolator<double, double, 5> interpolator5(points5, points5y);
        std::array<double, 1000> x = linspace<double, 1000>(0, length_array[i]);
        for (int j = 0; j < 1000; ++j) {
            data << std::setprecision(16) << interpolator5.interpolate(x[j])
                 << std::endl;
        }
    }
    data.close();

    std::array<double, 4> aboba = chebishev<double, 4>(-1, 1);
    std::cout << aboba[2];
}