#include "nonlinear_equations.h"
#include <array>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

Eigen::Vector<double, 2> func(Eigen::Vector<double, 2> x) {
    return {x[0] * x[0] + x[1] * x[1] - 1, x[1] - std::tan(x[0])};
};

Eigen::Vector<double, 2> func2(Eigen::Vector<double, 2> x) {
    return {x[0] * x[0] + x[1] * x[1] - 1, -x[1] + std::tan(x[0])};
};

int main() {
    // std::ofstream data;
    // data << std::setprecision(16);
    // data.open("e01.txt");
    // for (unsigned int i = 0; i < 100; ++i) {
    //     data << i << " " << keplerSolver(0.1, M_PI / 4., i, -1) << std::endl;
    // }
    // data.close();

    // data.open("e02.txt");
    // for (unsigned int i = 0; i < 100; ++i) {
    //     data << i << " " << keplerSolver(0.2, M_PI / 4., i, -1) << std::endl;
    // }
    // data.close();

    // data.open("e05.txt");
    // for (unsigned int i = 0; i < 100; ++i) {
    //     data << i << " " << keplerSolver(0.5, M_PI / 4., i, -1) << std::endl;
    // }
    // data.close();

    // data.open("e08.txt");
    // for (unsigned int i = 0; i < 100; ++i) {
    //     data << i << " " << keplerSolver(0.8, M_PI / 4., i, -1) << std::endl;
    // }
    // data.close();

    std::cout << solve<decltype(func), double>(func, -0.01, {0.7, 0.7}, 500)
              << std::endl;
    std::cout << solve<decltype(func2), double>(func2, 0.1, {-0.7, -0.7}, 5000)
              << std::endl;
}