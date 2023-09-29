#include "spline.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(ThreeDiagonalMatrix, InitializationAndGetValues) {
    ThreeDiagonalMatrix<double> matrix1({{0, 2, 3}, {10, 11, 12}, {-2, 5, 0}});
    ASSERT_EQ(matrix1(0, 0), 2);
    ASSERT_EQ(matrix1(2, 2), 5);
    ASSERT_EQ(matrix1(4, 0), 0);
}

TEST(ThreeDiagonalMatrix, SolvingSLAE) {
    ThreeDiagonalMatrix<double> matrix1(
        {{0, -2, 1}, {1, -2, 1}, {1, -2, 1}, {1, -2, 0}});
    std::vector<double> res1 =
        solve<double, double>(matrix1, {0.04, 0.04, 0.04, 0.04});

    const std::vector<double> solution1{-0.08, -0.12, -0.12, -0.08};
    for (int i = 0; i < res1.size(); ++i) {
        ASSERT_NEAR(res1[i], solution1[i], ERROR);
    }

    ThreeDiagonalMatrix<double> matrix2(
        {{0, 2, 3}, {6, 3, 9}, {2, 5, 2}, {4, 3, 0}});
    std::vector<double> res2 = solve<double, double>(matrix2, {21, 69, 34, 22});

    const std::vector<double> solution2{3, 5, 4, 2};
    for (int i = 0; i < res2.size(); ++i) {
        ASSERT_NEAR(res2[i], solution2[i], ERROR);
    }
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}