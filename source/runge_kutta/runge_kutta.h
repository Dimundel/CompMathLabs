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