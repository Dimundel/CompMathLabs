#include <array>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

int factorial(const int n) {
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

std::vector<double> linspace(const double start, const double end,
                             const unsigned int N) {
    std::vector<double> res;
    double delta = (end - start) / N;
    ;
    for (int i = 0; i < N; ++i) {
        res.push_back(start + i * delta);
    }
    return res;
}

template <typename RealType, unsigned int N> struct DerivativeCoef {
    RealType centralCoef;
    std::array<RealType, N> otherCoefs;
};

template <typename RealType, unsigned int N>
DerivativeCoef<RealType, N>
calcDerivativeCoef(const std::array<RealType, N> &points) noexcept {
    Eigen::Matrix<RealType, N + 1, 1>b =
        Eigen::Matrix<RealType, N + 1, 1>::Zero(N + 1);
    b(1) = 1;
    Eigen::Matrix<RealType, N + 1, N + 1> A =
        Eigen::Matrix<RealType, N + 1, N + 1>::Zero(N + 1, N + 1);
    for (int i = 0; i <= N; ++i) {
        A(0, i) = 1;
    }
    for (int j = 1; j <= N; ++j) {
        for (int i = 0; i < N; ++i) {
            A(j, i + 1) = std::pow(points[i], j) / factorial(j);
        }
    }
    Eigen::Matrix<RealType, N + 1, 1> v_res = A.colPivHouseholderQr().solve(b);

    std::array<RealType, N> res;
    for (int i = 0; i < N; ++i) {
        res[i] = v_res[i + 1];
    }
    return (DerivativeCoef<RealType, N>){v_res[0], res};
}

template <typename RealType, unsigned int N>
RealType exp_derivative(const RealType x_0, const RealType h,
                        const std::array<RealType, N> &points) {
    DerivativeCoef<RealType, N> coefs = calcDerivativeCoef<RealType, N>(points);
    RealType res = std::exp(x_0) * coefs.centralCoef / h;
    for (int i = 0; i < points.size(); ++i) {
        res += std::exp(x_0 + points[i] * h) * coefs.otherCoefs[i] / h;
    }
    return res;
}

int main() {
    const double x_0 = 1;
    const unsigned int N = 2;
    const std::array<double, 16> step{1,     1e-1,  1e-2,  1e-3, 1e-4,  1e-5,
                                      1e-6,  1e-7,  1e-8,  1e-9, 1e-10, 1e-11,
                                      1e-12, 1e-13, 1e-14, 1e-15};
    const std::array<double, N> points{-1, 1};
    for (int j = 0; j < 16; ++j) {
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
    for (int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(
                    exp_derivative<double, 3>(x_0, std::exp(x[i]), {-1, 1, 2}) -
                    EULER_CONSTANT))
             << std::endl;
    }
    data.close();

    data.open("dataN4.txt");
    for (int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(exp_derivative<double, 4>(x_0, std::exp(x[i]),
                                                            {-2, -1, 1, 2}) -
                                  EULER_CONSTANT))
             << std::endl;
    }
    data.close();

    data.open("dataN5.txt");
    for (int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(exp_derivative<double, 5>(x_0, std::exp(x[i]),
                                                            {-2, -1, 1, 2, 3}) -
                                  EULER_CONSTANT))
             << std::endl;
    }
    data.close();
}