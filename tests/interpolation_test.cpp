#include "interpolation.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(NewtonInterpolation, Interpolation) {
    NewtonInterpolator<double, double, 2> test1({-2, 3}, {0, 1});
    ASSERT_NEAR(test1.interpolate(-2), 0, ERROR);
    ASSERT_NEAR(test1.interpolate(3), 1, ERROR);
    ASSERT_NEAR(test1.interpolate(0), 0.4, ERROR);
    NewtonInterpolator<double, double, 4> test2({-4, 0, 5, 8}, {0, 23, -6, 40});
    ASSERT_NEAR(test2.interpolate(-4), 0, ERROR);
    ASSERT_NEAR(test2.interpolate(0), 23, ERROR);
    ASSERT_NEAR(test2.interpolate(5), -6, ERROR);
    ASSERT_NEAR(test2.interpolate(8), 40, ERROR);
    ASSERT_NEAR(test2.interpolate(1), 379.0 / 24, ERROR);
}

TEST(HermiteInterpolation, Interpolation) {
    HermiteInterpolator<double, double, 3> test1({0, 1, 3}, {2, 4, 5},
                                                 {1, -1, -2});
    HermiteInterpolator<double, double, 3> test2({-1, 0, 1}, {1, 2, 3},
                                                 {2, 2, 2});
    ASSERT_NEAR(test1.interpolate(2), 7. / 3, ERROR);
    ASSERT_NEAR(test1.interpolate(5), -1004. / 3, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}