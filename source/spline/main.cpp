#include "spline.h"
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

template <typename RealType>
std::vector<RealType> linspace(const double start, const double end,
                               unsigned int num) {
    std::vector<RealType> res;
    res.reserve(num);
    double delta = (end - start) / (num - 1);
    for (unsigned int i = 0; i < num; ++i) {
        res.push_back(start + i * delta);
    }
    return res;
}

template <typename RealType>
std::vector<double> exp(const std::vector<double> &x) {
    std::vector<RealType> res;
    res.reserve(x.size());
    for (unsigned int i = 0; i < x.size(); ++i) {
        res.push_back(std::exp(x[i]));
    }
    return res;
}

int main() {

    std::vector<double> x = linspace<double>(0, 10, 1000);

    // N = 5
    std::vector<double> x5(linspace<double>(0, 10, 5));
    std::vector<double> y5(exp<double>(x5));
    CubicSpline<double, double> spline5(x5, y5);

    std::ofstream data;
    data.open("data5.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16) << spline5.interpolate(x[i]) << std::endl;
    }
    data.close();

    // N = 10
    std::vector<double> x10(linspace<double>(0, 10, 10));
    std::vector<double> y10(exp<double>(x10));
    CubicSpline<double, double> spline10(x10, y10);

    data.open("data10.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16) << spline10.interpolate(x[i])
             << std::endl;
    }
    data.close();

    // N = 20
    std::vector<double> x20(linspace<double>(0, 10, 20));
    std::vector<double> y20(exp<double>(x20));
    CubicSpline<double, double> spline20(x20, y20);

    data.open("data20.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16) << spline20.interpolate(x[i])
             << std::endl;
    }
    data.close();

    // N = 40
    std::vector<double> x40(linspace<double>(0, 10, 40));
    std::vector<double> y40(exp<double>(x40));
    CubicSpline<double, double> spline40(x40, y40);

    data.open("data40.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16) << spline40.interpolate(x[i])
             << std::endl;
    }
    data.close();

    // N = 80
    std::vector<double> x80(linspace<double>(0, 10, 80));
    std::vector<double> y80(exp<double>(x80));
    CubicSpline<double, double> spline80(x80, y80);

    data.open("data80.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16) << spline80.interpolate(x[i])
             << std::endl;
    }
    data.close();

    // N = 160
    std::vector<double> x160(linspace<double>(0, 10, 160));
    std::vector<double> y160(exp<double>(x160));
    CubicSpline<double, double> spline160(x160, y160);

    data.open("data160.txt");
    for (unsigned int i = 0; i < 1000; ++i) {
        data << std::setprecision(16) << spline160.interpolate(x[i])
             << std::endl;
    }
    data.close();
}