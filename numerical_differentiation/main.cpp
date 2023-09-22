#include "numerical_differentiation.h"

int main() {
    const double x_0 = 1;
    const unsigned int N = 2;
    const std::array<double, 16> step{1,     1e-1,  1e-2,  1e-3, 1e-4,  1e-5,
                                      1e-6,  1e-7,  1e-8,  1e-9, 1e-10, 1e-11,
                                      1e-12, 1e-13, 1e-14, 1e-15};
    const std::array<double, N> points{-1, 1};
    for (unsigned int j = 0; j < 16; ++j) {
        std::cout << std::setprecision(16)
                  << exp_derivative<double, 2>(x_0, step[j], points)
                  << std::endl;
    }

    // Task 2
    const double EULER_CONSTANT = std::exp(1);
    std::ofstream data;
    double start = std::log(1e-15);
    double end = std::log(1);
    const unsigned int num = 10000;
    std::vector<double> x = linspace(start, end, num);
    data.open("dataN3.txt");
    for (unsigned int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(
                    exp_derivative<double, 3>(x_0, std::exp(x[i]), {-1, 1, 2}) -
                    EULER_CONSTANT))
             << std::endl;
    }
    data.close();

    data.open("dataN4.txt");
    for (unsigned int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(exp_derivative<double, 4>(x_0, std::exp(x[i]),
                                                            {-2, -1, 1, 2}) -
                                  EULER_CONSTANT))
             << std::endl;
    }
    data.close();

    data.open("dataN5.txt");
    for (unsigned int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(exp_derivative<double, 5>(x_0, std::exp(x[i]),
                                                            {-2, -1, 1, 2, 3}) -
                                  EULER_CONSTANT))
             << std::endl;
    }
    data.close();
}