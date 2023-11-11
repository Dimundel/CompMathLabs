#include <algorithm>
#include <array>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <numeric>
#include <vector>

/* Это таблица Бутчера для метода Рунге-Кутты 4 порядка. Я ее не заполнил */

// а я заполнил

struct RK4Table {
    static constexpr unsigned int stages = 4;
    static constexpr std::array<std::array<double, stages>, stages> table = {
        {{0, 0, 0, 0}, {1. / 2, 0, 0, 0}, {0, 1. / 2, 0, 0}, {0, 0, 1, 0}}};
    static constexpr std::array<double, stages> cColumn = {0, 1. / 2., 1. / 2.,
                                                           1};
    static constexpr std::array<double, stages> bString = {1. / 6., 1. / 3,
                                                           1. / 3., 1. / 6.};
};

class Cube {

public:
    static constexpr unsigned int dim = 1; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = Eigen::Vector<double, dim>; // состояние

    struct StateAndArg {
        State state;
        Argument arg;
    };

    /*** Вычисляет правую часть ДУ - функцию f***/
    Eigen::Vector<double, dim> calc(const StateAndArg &stateAndArg) const {
        return Eigen::Vector<double, dim>{stateAndArg.arg * stateAndArg.arg *
                                          stateAndArg.arg};
    }
};

class Oscillator {

public:
    static constexpr unsigned int dim = 2; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = Eigen::Vector<double, dim>; // состояние

    struct StateAndArg {
        State state;
        Argument arg;
    };

    /*** Вычисляет правую часть ДУ - функцию f***/
    Eigen::Vector<double, dim> calc(const StateAndArg &stateAndArg) const {
        return Eigen::Vector<double, dim>{stateAndArg.state(1),
                                          -stateAndArg.state(0)};
    }
};

class ArenstorfOrbit {

private:
    const double k1 = 0.012277471;
    const double k2 = 1 - k1;

public:
    static constexpr unsigned int dim = 4; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = Eigen::Vector<double, dim>; // состояние

    struct StateAndArg {
        State state;
        Argument arg;
    };

    /*** Вычисляет правую часть ДУ - функцию f***/
    Eigen::Vector<double, dim> calc(const StateAndArg &stateAndArg) const {
        return Eigen::Vector<double, dim>{
            stateAndArg.state(2), stateAndArg.state(3),
            stateAndArg.state(0) + 2 * stateAndArg.state(3) -
                k2 * (stateAndArg.state(0) + k1) /
                    (std::pow(((stateAndArg.state(0) + k1) *
                                   (stateAndArg.state(0) + k1) +
                               stateAndArg.state(1) * stateAndArg.state(1)),
                              3. / 2.)) -
                k1 * (stateAndArg.state(0) - k2) /
                    (std::pow(((stateAndArg.state(0) - k2) *
                                   (stateAndArg.state(0) - k2) +
                               stateAndArg.state(1) * stateAndArg.state(1)),
                              3. / 2.)),
            stateAndArg.state(1) - 2 * stateAndArg.state(2) -
                k2 * stateAndArg.state(1) /
                    (std::pow(((stateAndArg.state(0) + k1) *
                                   (stateAndArg.state(0) + k1) +
                               stateAndArg.state(1) * stateAndArg.state(1)),
                              3. / 2.)) -
                k1 * stateAndArg.state(1) /
                    (std::pow(((stateAndArg.state(0) - k2) *
                                   (stateAndArg.state(0) - k2) +
                               stateAndArg.state(1) * stateAndArg.state(1)),
                              3. / 2.))};
    }
};

template <typename Table,
          typename RHS> // таблица бутчера и класс правой части f
std::vector<typename RHS::StateAndArg>
integrate(const typename RHS::StateAndArg &initialState,
          const typename RHS::Argument &endTime, double step, const RHS &rhs) {
    unsigned int num = std::ceil((endTime - initialState.arg) / step);
    step = (endTime - initialState.arg) / num;
    Eigen::Matrix<double, RHS::dim, Table::stages> k;
    std::vector<typename RHS::StateAndArg> res;
    res.reserve(num + 1);
    res.push_back(initialState);
    Eigen::Vector<double, RHS::dim> temp;
    Eigen::Vector<double, RHS::dim> sum_of_k;
    for (std::size_t i = 1; i < num + 1; ++i) {
        for (std::size_t j = 0; j < Table::stages; ++j) {
            temp = Eigen::Vector<double, RHS::dim>::Zero();
            for (std::size_t n = 0; n < Table::stages; ++n) {
                temp += step * Table::table[j][n] * k.col(n);
            }
            k.col(j) = rhs.calc({res.back().state + temp,
                                 res.back().arg + step * Table::cColumn[j]});
        }
        sum_of_k = Eigen::Vector<double, RHS::dim>::Zero();
        for (std::size_t j = 0; j < Table::stages; ++j) {
            sum_of_k += k.col(j) * Table::bString[j];
        }
        res.push_back(
            {res.back().state + step * sum_of_k, res.back().arg + step});
    }
    return res;
}

struct DP45 {
    static constexpr unsigned int stages = 7;
    static constexpr std::array<std::array<double, stages>, stages> table = {
        {{0, 0, 0, 0, 0, 0, 0},
         {1. / 5., 0, 0, 0, 0, 0, 0},
         {3. / 40., 9. / 40., 0, 0, 0, 0, 0},
         {44. / 45., -56. / 15., 32. / 9., 0, 0, 0, 0},
         {19372. / 6561., -25360. / 2187., 64448. / 6561., -212. / 729., 0, 0,
          0},
         {9017. / 3168., -355. / 33., 46732. / 5247., 49. / 176.,
          -5103. / 18656., 0, 0},
         {35. / 384., 0, 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84.,
          0}}};
    static constexpr std::array<double, stages> cColumn = {
        0, 1. / 5., 3. / 10., 4. / 5., 8. / 9., 1., 1.};
    static constexpr std::array<double, stages> bString1 = {
        35. / 384., 0, 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0};
    static constexpr std::array<double, stages> bString2 = {
        5179. / 57600., 0,       7571. / 16695., 393. / 640., -92097. / 339200.,
        187. / 2100.,   1. / 40.};
    static constexpr unsigned int approximation = 5;
};

struct StepControl {
    double minStep;
    double maxStep;
    double tolerance;
    double initialStep;
};

template <typename Table,
          typename RHS> // таблица бутчера и класс правой части f
std::vector<typename RHS::StateAndArg>
integrate(const typename RHS::StateAndArg &initialState,
          const typename RHS::Argument &endTime, const StepControl &stepControl,
          const RHS &rhs) {
    double step = stepControl.initialStep;
    std::vector<typename RHS::StateAndArg> res;
    double currentTime = initialState.arg;
    // res.reserve();
    res.push_back(initialState);
    Eigen::Vector<double, RHS::dim> temp;
    Eigen::Matrix<double, RHS::dim, Table::stages> k;
    Eigen::Vector<double, RHS::dim> sum_of_k_4;
    Eigen::Vector<double, RHS::dim> sum_of_k_5;
    double error;
    while (currentTime < endTime) {
        for (std::size_t j = 0; j < Table::stages; ++j) {
            temp = Eigen::Vector<double, RHS::dim>::Zero();
            for (std::size_t n = 0; n < Table::stages; ++n) {
                temp += step * Table::table[j][n] * k.col(n);
            }
            k.col(j) = rhs.calc({res.back().state + temp,
                                 res.back().arg + step * Table::cColumn[j]});
        }
        sum_of_k_4 = res.back().state;
        sum_of_k_5 = res.back().state;
        for (std::size_t j = 0; j < Table::stages; ++j) {
            sum_of_k_5 += step * k.col(j) * Table::bString1[j];
            sum_of_k_4 += step * k.col(j) * Table::bString2[j];
        }
        error = (sum_of_k_4 - sum_of_k_5).norm();
        if (error > stepControl.tolerance) {
            step = std::clamp(
                step * std::pow(error / stepControl.tolerance, 1. / 5.),
                stepControl.minStep, stepControl.maxStep);
            continue;
        } else {
            res.push_back({sum_of_k_5, res.back().arg + step});
            currentTime += step;
            step = std::clamp(
                step * std::pow(error / stepControl.tolerance, 1. / 5.),
                stepControl.minStep, stepControl.maxStep);
        }
    }
    return res;
}