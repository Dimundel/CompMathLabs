#include "numerical_differentiation.h"

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

template <typename RealType, unsigned int N, unsigned int L>
RealType exp_derivative(const RealType x_0, const RealType h,
                        const std::array<RealType, N> &points) {
    DerivativeCoef<RealType, N> coefs = calcDerivativeCoef<RealType, N, L>(points);
    RealType res = std::exp(x_0) * coefs.centralCoef / std::pow(h, L);
    for (unsigned int i = 0; i < points.size(); ++i) {
        res += std::exp(x_0 + points[i] * h) * coefs.otherCoefs[i] /
               std::pow(h, L);
    }
    return res;
}

int main() {
    const double x_0 = 1;
    const double EULER_CONSTANT = std::exp(1);
    std::ofstream data;
    double start = std::log(1e-4);
    double end = std::log(100);
    const unsigned int num = 10000;
    std::vector<double> x = linspace(start, end, num);
    data.open("dataN3.txt");
    for (unsigned int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(exp_derivative<double, 3, 2>(
                                      x_0, std::exp(x[i]), {-1, 1, 2}) -
                                  EULER_CONSTANT))
             << std::endl;
    }
    data.close();

    data.open("dataN4.txt");
    for (unsigned int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(exp_derivative<double, 4, 2>(
                                      x_0, std::exp(x[i]), {-2, -1, 1, 2}) -
                                  EULER_CONSTANT))
             << std::endl;
    }
    data.close();

    data.open("dataN5.txt");
    for (unsigned int i = 0; i < num; ++i) {
        data << x[i] << " "
             << std::log(std::abs(exp_derivative<double, 5, 2>(
                                      x_0, std::exp(x[i]), {-2, -1, 1, 2, 3}) -
                                  EULER_CONSTANT))
             << std::endl;
    }
    data.close();
}