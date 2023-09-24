#include "interpolation.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(NewtonInterpolationCheck, InterpolationCheck) {
    NewtonInterpolator<double, double, 2> test1({-2, 3}, {0, 1});
    ASSERT_NEAR(test1.interpolate(-2), 0, ERROR);
    ASSERT_NEAR(test1.interpolate(3), 1, ERROR);
    ASSERT_NEAR(test1.interpolate(0), 0.4, ERROR);
    NewtonInterpolator<double, double, 4> test2({-4, 0, 5, 8}, {0, 23, -6, 40});
    ASSERT_NEAR(test2.interpolate(-4), 0, ERROR);
    ASSERT_NEAR(test2.interpolate(0), 23, ERROR);
    ASSERT_NEAR(test2.interpolate(5), -6, ERROR);
    ASSERT_NEAR(test2.interpolate(8), 40, ERROR);
    ASSERT_NEAR(test2.interpolate(1), 379.0/24, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}