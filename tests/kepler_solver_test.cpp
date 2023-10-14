#include "nonlinear_equations.h"
#include <cmath>
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(KeplerSovler, Solve) {
    ASSERT_NEAR(keplerSolver(0.1, M_PI / 4, 1000, 1e-5), 0.861265, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}