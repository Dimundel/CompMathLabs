#include "nonlinear_equations.h"
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <gtest/gtest.h>

const double ERROR = 1e-3;

double function1(const double x) { return std::exp(x) - x * x - 4; }

Eigen::Vector<double, 2> function2(Eigen::Vector<double, 2> x) {
    return {x[0] * x[0] + x[1] * x[1] - 1, x[1] - std::tan(x[0])};
};

TEST(SOR, SolveSystem) {
    auto res =
        solve<decltype(function2), double>(function2, -0.01, {0.7, 0.7}, 400);
    ASSERT_NEAR(res[0], 0.6499, ERROR);
    ASSERT_NEAR(res[1], 0.76, ERROR);
}

TEST(SOR, Solve) {
    auto res =
        solve<decltype(function1), double>(function1, -2. / 17.5, 2, 100);
    ASSERT_NEAR(res, 2.15873, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}