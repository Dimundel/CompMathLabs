#include "shooting_method.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

const IntegrationParameters parameters{0.01, 1e-5, -0.01, 10000};

class Foo {

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
                                          stateAndArg.state(0)};
    }
};

TEST(ShootingMethod, SolveOscillator) {
    RungeKutta::Oscillator oscillator;
    auto res = integrate<RungeKutta::RK4Table, RungeKutta::Oscillator>(
        {0, 0}, {M_PI / 2, 2}, -1, parameters, oscillator);
    ASSERT_NEAR(res.back().state[0], 2, ERROR);
};

TEST(ShootingMethod, SolveFoo) {
    Foo foo;
    auto res =
        integrate<RungeKutta::RK4Table, Foo>({0, 0}, {1, 1}, -1, parameters, foo);
    ASSERT_NEAR(res.back().state[0], 1., ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}