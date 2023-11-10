#include "runge_kutta.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(RungeKutta, SolveOscillator) {
    RK4Table table;
    Oscillator oscillator;
    auto res =
        integrate<RK4Table, Oscillator>({{0, 1}, 0}, 1, 0.01, oscillator);
    ASSERT_NEAR(res[100].state[0], 0.841471, ERROR);
    ASSERT_NEAR(res[100].state[1], 0.540302, ERROR);
}

TEST(RungeKutta, SolveCube) {
    RK4Table table;
    Cube cube;
    Eigen::Vector<double, 1> a{0};
    Cube::StateAndArg initCond{a, 0};
    auto res = integrate<RK4Table, Cube>(initCond, 5, 0.01, cube);
    ASSERT_NEAR(res[500].state[0], 156.25, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}