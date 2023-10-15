#include "nonlinear_equations.h"
#include <cmath>
#include <gtest/gtest.h>

const double ERROR = 1e-3;

double function1(const double x) { return std::exp(x) - x * x - 4; }

// TEST(SOR, Solve) {
//     ASSERT_NEAR(
//         solve<decltype(function1), double>(function1, 2. / 17.5, 0.01, 20),
//         2.159, ERROR);
// }

TEST(SOR, Solve) {
    auto res1 =
        solve<decltype(function1), double>(function1, -2. / 17.5, 2, 100);
    ASSERT_NEAR(res1, 2.15873, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}