#include "bdf.h"
#include <fstream>
#include <iomanip>

const Cube cube;
const Oscillator oscillator;

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
    IntegrationParameters param;
    std::ofstream data;
    // data.open("data_cube.txt");
    // const Eigen::Vector<double, 1> a{0};
    // const Cube::StateAndArg initCond{a, 0};
    // const std::array<double, 12> steps =
    //     powof10<double, 12>(linspace<double, 12>(-6, 1));
    // for (unsigned int i = 0; i < 12; ++i) {
    //     auto res = integrate<BDF4, Cube, RungeKutta::RK4Table>(
    //         initCond, 5, {steps[i], 0.01, 100}, cube);
    //     for (const auto &element : res) {
    //         data << std::setprecision(16) << element.arg << " "
    //              << element.state[0] << std::endl;
    //     }
    //     data << std::endl;
    // }
    // data.close();
    data.open("data_oscillator.txt");
    const Eigen::Vector<double, 2> a{0, 1};
    const Oscillator::StateAndArg initCond{a, 0};
    const std::array<double, 12> steps =
        powof10<double, 12>(linspace<double, 12>(-6, 1));
    for (unsigned int i = 0; i < 12; ++i) {
        auto res = integrate<BDF4, Oscillator, RungeKutta::RK4Table>(
            initCond, 5, {steps[i], 0.01, 100}, oscillator);
        for (const auto &element : res) {
            data << std::setprecision(16) << element.arg << " "
                 << element.state[0] << " " << element.state[1] <<std::endl;
        }
        data << std::endl;
    }
}