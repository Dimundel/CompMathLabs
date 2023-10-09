#include "numerical_differentiation.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(DerivativeCheck, DerivativeCoefCheck1) {
    const double x_0 = 1;
    const unsigned int N = 2;
    const std::array<double, N> points{-1, 1};
    const DerivativeCoef res = calcDerivativeCoef<double, N>(points);
    ASSERT_NEAR(res.centralCoef, 0.0, ERROR);
    const std::array<double, N> ans{-0.5, 0.5};
    for (int i = 0; i < N; ++i) {
        ASSERT_NEAR(res.otherCoefs[i], ans[i], ERROR);
    }
}

TEST(DerivativeCheck, DerivativeCoefCheck2) {
    const double x_0 = 1;
    const unsigned int N = 3;
    const std::array<double, N> points{-1, 1, 2};
    const DerivativeCoef res = calcDerivativeCoef<double, N>(points);
    ASSERT_NEAR(res.centralCoef, -1.0 / 2, ERROR);
    const std::array<double, N> ans{-1.0 / 3, 1.0, -1.0 / 6};
    for (int i = 0; i < N; ++i) {
        ASSERT_NEAR(res.otherCoefs[i], ans[i], ERROR);
    }
}

TEST(DerivativeCheck, DerivativeSecondOrder) {
    const double x_0 = 1;
    const unsigned int N = 2;
    const std::array<double, N> points{-1, 1};
    const DerivativeCoef res = calcDerivativeCoef<double, N, 2>(points);
    ASSERT_NEAR(res.centralCoef, -2, ERROR);
    const std::array<double, N> ans{1, 1};
    for (int i = 0; i < N; ++i) {
        ASSERT_NEAR(res.otherCoefs[i], ans[i], ERROR);
    }
}

TEST(DerivativeCheck, DerivativeThirdOrder) {
    const double x_0 = 0;
    const unsigned int N = 3;
    const std::array<double, N> points{-1, 1, 2};
    const DerivativeCoef res = calcDerivativeCoef<double, N, 3>(points);
    ASSERT_NEAR(res.centralCoef, 3, ERROR);
    const std::array<double, N> ans{-1, -3, 1};
    for (int i = 0; i < N; ++i) {
        ASSERT_NEAR(res.otherCoefs[i], ans[i], ERROR);
    }
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}