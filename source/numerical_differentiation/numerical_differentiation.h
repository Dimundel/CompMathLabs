#include <array>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

std::vector<double> linspace(const double start, const double end,
                             const unsigned int N) {
    std::vector<double> res;
    double delta = (end - start) / N;
    ;
    for (unsigned int i = 0; i < N; ++i) {
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
    Eigen::Matrix<RealType, N + 1, 1> b =
        Eigen::Matrix<RealType, N + 1, 1>::Zero(N + 1);
    b(1) = 1;
    Eigen::Matrix<RealType, N + 1, N + 1> A =
        Eigen::Matrix<RealType, N + 1, N + 1>::Zero(N + 1, N + 1);
    for (unsigned int i = 0; i <= N; ++i) {
        A(0, i) = 1;
    }
    for (unsigned int j = 1; j <= N; ++j) {
        for (unsigned int i = 0; i < N; ++i) {
            A(j, i + 1) = A(j - 1, i + 1) * points[i] / j;
        }
    }
    Eigen::Matrix<RealType, N + 1, 1> v_res = A.colPivHouseholderQr().solve(b);

    std::array<RealType, N> res;
    for (unsigned int i = 0; i < N; ++i) {
        res[i] = v_res[i + 1];
    }
    return (DerivativeCoef<RealType, N>){v_res[0], res};
}

template <typename RealType, unsigned int N>
RealType exp_derivative(const RealType x_0, const RealType h,
                        const std::array<RealType, N> &points) {
    DerivativeCoef<RealType, N> coefs = calcDerivativeCoef<RealType, N>(points);
    RealType res = std::exp(x_0) * coefs.centralCoef / h;
    for (unsigned int i = 0; i < points.size(); ++i) {
        res += std::exp(x_0 + points[i] * h) * coefs.otherCoefs[i] / h;
    }
    return res;
}