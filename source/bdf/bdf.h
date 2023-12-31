#ifndef BDF_H
#define BDF_H

#include "../runge_kutta/runge_kutta.h"
#include <eigen3/Eigen/Dense>
#include <functional>
#include <iostream>

/* Это коэффициенты для метода на 4 шагах. Я их не заполнил */
struct BDF4 {
    static constexpr unsigned int size = 4;
    static constexpr std::array<double, size + 1> alpha = {
        -3. / 25, 16. / 25, -36. / 25, 48. / 25, 12. / 25};
};

template <typename BDF, typename RHS>
Eigen::Vector<double, RHS::dim>
BDF_func(const RHS &rhs, const double h,
         const std::array<Eigen::Vector<double, RHS::dim>, BDF::size> &mat,
         const typename RHS::StateAndArg &x) {
    Eigen::Vector<double, RHS::dim> res = h * rhs.calc(x) * BDF::alpha.back();
    for (std::size_t i = 0; i < BDF::size; ++i) {
        res += mat[i] * BDF::alpha[i];
    }
    return res;
}

class Oscillator {

public:
    static constexpr unsigned int dim = 2; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = Eigen::Vector<double, dim>; // состояние

    struct StateAndArg {
        State state;
        Argument arg;
    };

    /*** Вычисляет разницу двух состояний (решения нелиненого уравнения) ***/
    State calcDif(const State &first, const State &second) const {
        return second - first;
    }

    /*** Вычисляет правую часть ДУ - функцию f***/
    Eigen::Vector<double, dim> calc(const StateAndArg &stateAndArg) const {
        return Eigen::Vector<double, dim>{stateAndArg.state(1),
                                          -stateAndArg.state(0)};
    }
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

    /*** Вычисляет разницу двух состояний (решения нелиненого уравнения) ***/
    State calcDif(const State &first, const State &second) const {
        return second - first;
    }

    /*** Вычисляет правую часть ДУ - функцию f***/
    Eigen::Vector<double, dim> calc(const StateAndArg &stateAndArg) const {
        return Eigen::Vector<double, dim>{stateAndArg.arg * stateAndArg.arg *
                                          stateAndArg.arg};
    }
};

class EarthOrbit {
private:
    const double mu = 398600.4415888;
    const double epsilon = 3. / 2. * mu * 6378.14 * 6378.14 * 1.082 / 1000;

public:
    static constexpr unsigned int dim = 6; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = Eigen::Vector<double, dim>; // состояние

    struct StateAndArg {
        State state;
        Argument arg;
    };

    /*** Вычисляет разницу двух состояний (решения нелиненого уравнения) ***/
    State calcDif(const State &first, const State &second) const {
        return second - first;
    }

    /*** Вычисляет правую часть ДУ - функцию f***/
    Eigen::Vector<double, dim> calc(const StateAndArg &stateAndArg) const {
        return Eigen::Vector<double, dim>{
            stateAndArg.state[3],
            stateAndArg.state[4],
            stateAndArg.state[5],
            -mu / (stateAndArg.state[0] * stateAndArg.state[0]) +
                epsilon /
                    (stateAndArg.state[0] * stateAndArg.state[0] *
                     stateAndArg.state[0] * stateAndArg.state[0]) *
                    (3 * std::sin(stateAndArg.state[2]) *
                         std::sin(stateAndArg.state[2]) -
                     1),
            0,
            -epsilon /
                (stateAndArg.state[0] * stateAndArg.state[0] *
                 stateAndArg.state[0] * stateAndArg.state[0]) *
                std::sin(2 * stateAndArg.state[2])};
    }
};

struct IntegrationParameters {
    double step; // шаг интегрирования
    double epsilon; // точность решения нелинейного уравнения
    unsigned int maxIter; // максимальное количество итераций для
                          // решения нелинейного уравнения
};

template <typename RHS>
Eigen::Vector<double, RHS::dim>
predictor_corrector(const typename RHS::StateAndArg &stateAndArg,
                    const double h, const RHS &rhs) {
    Eigen::Vector<double, RHS::dim> p =
        stateAndArg.state + h * rhs.calc(stateAndArg);
    Eigen::Vector<double, RHS::dim> k = rhs.calc({p, stateAndArg.arg + h});
    Eigen::Vector<double, RHS::dim> c =
        stateAndArg.state + h / 2 * (k + rhs.calc(stateAndArg));
    return c;
}

template <typename RHS>
Eigen::Vector<double, RHS::dim> fixed_point_iteration(
    const std::function<Eigen::Vector<double, RHS::dim>(
        const Eigen::Vector<double, RHS::dim> &x)>
        func,
    const IntegrationParameters &parameters, const RHS &rhs,
    const typename Eigen::Vector<double, RHS::dim> &initGuess) {
    Eigen::Vector<double, RHS::dim> solution = func(initGuess);
    Eigen::Vector<double, RHS::dim> solution_next;
    for (std::size_t count = 0; count < parameters.maxIter; ++count) {
        solution_next = func(solution);
        if (rhs.calcDif(solution_next, solution).norm() < parameters.epsilon) {
            break;
        }
        solution = solution_next;
    }
    return solution_next;
}

/***
BDF - структура с коэффициентами метода
RHS - правая часть Д.У.
RKTable -  таблица Бутчера метода Рунге-Кутты для разгона.
***/

template <typename BDF, typename RHS,
          typename RKTable> // таблица бутчера и класс правой части f
std::vector<typename RHS::StateAndArg>
integrate(const typename RHS::StateAndArg &initialState,
          const typename RHS::Argument &endTime,
          const IntegrationParameters &parameters, const RHS &rhs) {
    unsigned int num =
        std::ceil((endTime - initialState.arg) / parameters.step);
    double step = (endTime - initialState.arg) / num;
    std::vector<typename RHS::StateAndArg> res =
        RungeKutta::integrate<RKTable, RHS>(
            initialState, initialState.arg + step * (BDF::size - 1), step, rhs);
    Eigen::Vector<double, RHS::dim> initGuess;
    double next_time = initialState.arg + step * 4;

    for (std::size_t i = 4; i < num + 1; ++i) {
        auto func = [&res, step, &rhs,
                     next_time](const Eigen::Vector<double, RHS::dim> &x)
            -> Eigen::Vector<double, RHS::dim> {
            const typename RHS::StateAndArg a = {x, next_time};
            Eigen::Vector<double, RHS::dim> temp =
                step * rhs.calc(a) * BDF::alpha.back();
            for (std::size_t i = 0; i < BDF::size; ++i) {
                temp += res[res.size() - 4 + i].state * BDF::alpha[i];
            }
            return temp;
        };
        initGuess = predictor_corrector<RHS>(res.back(), step, rhs);
        res.push_back(
            {fixed_point_iteration<RHS>(func, parameters, rhs, initGuess),
             next_time});
        next_time += step;
    }
    return res;
}

#endif