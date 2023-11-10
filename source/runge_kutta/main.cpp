#include "runge_kutta.h"
#include <fstream>
#include <iomanip>
#include <iostream>

const RK4Table table;
const Cube cube;
const Oscillator oscillator;
double START = 0.;
double END = 5.;

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
std::array<RealType, N> powof10(const std::array<double, N> &x) {
    std::array<double, N> y;
    for (int i = 0; i < x.size(); ++i) {
        y[i] = std::pow(10, x[i]);
    }
    return y;
}

int main() {
    std::ofstream data;
    // data.open("data_cube.txt");
    const std::array<double, 6> steps =
        powof10<double, 6>({-5, -4, -3, -2, -1, 0});
    // for (unsigned int i = 0; i < 6; ++i) {
    //     auto res = integrate<RK4Table, Cube>({{0}, START}, END, steps[i], cube);
    //     for (const auto &element : res) {
    //         data << std::setprecision(16) << element.arg << " "
    //              << element.state[0] << std::endl;
    //     }
    //     data << std::endl;
    // }
    // data.close();

    data.open("data_oscillator.txt");
    for (unsigned int i = 0; i < 6; ++i) {
        auto res = integrate<RK4Table, Oscillator>({{0, 1}, START}, END, steps[i],
                                                   oscillator);
        for (const auto &element : res) {
            data << std::setprecision(16) << element.arg << " "
                 << element.state[0] << " " << element.state[1] << std::endl;
        }
        data << std::endl;
    }
    data.close();
}