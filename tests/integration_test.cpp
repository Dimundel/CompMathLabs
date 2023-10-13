#include "integration.h"
#include <functional>
#include <gtest/gtest.h>

const double ERROR = 1e-6;
double func_square(const double x) { return x * x; }
double some_polynomial(const double x) {
    return 4 * x * x * x - 3 * x * x - 5 * x + 6;
}

TEST(Integration, IntegrationDX) {
    auto res1 = integrate<decltype(func_square), double, 3>(func_square, -2, 2);
    ASSERT_NEAR(res1, 5.33333, ERROR);

    auto res2 =
        integrate<decltype(func_square), double, 5>(some_polynomial, 4, 8);
    ASSERT_NEAR(res2, 3296, ERROR);
}

TEST(Integration, Integration) {
    auto res1 =
        integrate<decltype(func_square), double, 4>(func_square, -4, -2, 0.1);
    ASSERT_NEAR(res1, 18.667, ERROR);

    auto res2 = integrate<decltype(some_polynomial), double, 5>(some_polynomial,
                                                                -2, 3, 0.1);
    ASSERT_NEAR(res2, 47.5, ERROR);
}

TEST(RungeRichardsonIntegration, Integration) {
    auto res1 = integrateRungeRichardson<decltype(func_square), double, 5>(
        func_square, -4, -2, 1e-20);
    ASSERT_NEAR(res1, 56. / 3., ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}