#include "bdf.h"
#include <gtest/gtest.h>

const double ERROR = 1e-1;

TEST(BDF, IntegrateOscillator) {
    Oscillator oscillator;
    auto res = integrate<BDF4, Oscillator, RungeKutta::RK4Table>(
        {{0, 1}, 0}, 1, {0.1, 1e-5, 500}, oscillator);
    ASSERT_NEAR(res.back().state[0], 0.84147098480, ERROR);
    ASSERT_NEAR(res.back().state[1], 0.54030230586, ERROR);
}

TEST(BDF, IntegrateCube) {
    Cube cube;
    Eigen::Vector<double, 1> a{0};
    Cube::StateAndArg b = {a, 0};
    auto res = integrate<BDF4, Cube, RungeKutta::RK4Table>(
        b, 1, {0.01, 1e-3, 100}, cube);
    ASSERT_NEAR(res.back().state[0], 0.25, ERROR);
}

int main() {
    // Oscillator oscillator;
    // auto res = integrate<BDF4, Oscillator, RungeKutta::RK4Table>(
    //     {{0, 1}, 0}, 1, {0.1, 1e-3, 100}, oscillator);
    // for (const auto &element : res) {
    //     std::cout << element.arg << " " << element.state[0] << std::endl;
    // }
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}