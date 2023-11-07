#include <array>
#include <cmath>
#include <iostream>
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

template <typename T, std::size_t N>
std::array<T, N> operator+(const std::array<T, N> &v1,
                           const std::array<T, N> &v2) {
    std::array<T, N> res;
    for (std::size_t i = 0; i < N; ++i) {
        res[i] = v1[i] + v2[i];
    }
    return res;
}

template <typename T, std::size_t N, std::size_t M>
std::array<T, N> operator*(const std::array<std::array<T, N>, M> &mat,
                           const std::array<T, M> &vec) {
    std::array<T, N> res;
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            res[j] += mat[i][j] * vec[i];
        }
    }
    return res;
}

template <typename T, std::size_t N>
std::array<T, N> operator*(const std::array<T, N> &vec, const double n) {
    std::array<T, N> res;
    for (std::size_t i = 0; i < N; ++i) {
        res[i] = vec[i] * n;
    }
    return res;
}

template <typename T, std::size_t N>
std::array<T, N> operator*(const double n, const std::array<T, N> &vec) {
    return vec * n;
}

class Cube {

public:
    static constexpr unsigned int dim = 1; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = std::array<double, dim>; // состояние

    struct StateAndArg {
        State state;
        Argument arg;
    };

    /*** Вычисляет правую часть ДУ - функцию f***/
    std::array<double, dim> calc(const StateAndArg &stateAndArg) const {
        return {stateAndArg.arg * stateAndArg.arg * stateAndArg.arg};
    }
};

class Oscillator {

public:
    static constexpr unsigned int dim = 2; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = std::array<double, dim>; // состояние

    struct StateAndArg {
        State state;
        Argument arg;
    };

    /*** Вычисляет правую часть ДУ - функцию f***/
    std::array<double, dim> calc(const StateAndArg &stateAndArg) const {
        return {stateAndArg.state[1], -stateAndArg.state[0]};
    }
};

template <typename Table,
          typename RHS> // таблица бутчера и класс правой части f
std::vector<typename RHS::StateAndArg>
integrate(const typename RHS::StateAndArg &initialState,
          const typename RHS::Argument &endTime, double step, const RHS &rhs) {
    unsigned int num = std::ceil((endTime - initialState.arg) / step);
    step = (endTime - initialState.arg) / num;
    std::array<std::array<double, RHS::dim>, Table::stages> k;
    std::vector<typename RHS::StateAndArg> res;
    res.reserve(num + 1);
    res.push_back(initialState);
    for (unsigned int i = 1; i < num + 1; ++i) {
        for (unsigned int j = 0; j < Table::stages; ++j) {
            k[j] = rhs.calc({res.back().state + k * Table::table[j] * step,
                             res.back().arg + step * Table::cColumn[j]});
        }
        res.push_back({res.back().state + k * Table::bString * step,
                       res.back().arg + step});
    }
    return res;
}