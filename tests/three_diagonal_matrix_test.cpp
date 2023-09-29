#include "spline.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(ThreeDiagonalMatrix, InitializationAndGetValues) {
    ThreeDiagonalMatrix<double> matrix1({{0, 2, 3}, {10, 11, 12}, {-2, 5, 0}});
    ASSERT_EQ(matrix1(0, 0), 2);
    ASSERT_EQ(matrix1(2, 2), 5);
    ASSERT_EQ(matrix1(4, 0), 0);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}