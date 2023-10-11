#include "spline.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(SplineTest, NaturalCubicSplineInterpolation) {
    CubicSpline<double, double> spline({-1, 0, 1}, {1, 2, 5}, 0, 0);
    ASSERT_NEAR(spline.interpolate(0), 2, ERROR);
}

TEST(SplineTest, GeneralCubicSplineInterpolation) {
    CubicSpline<double, double> spline({-1, 0, 0.75}, {5, 3, 4.26563}, 0, 1.5);
    ASSERT_NEAR(spline.interpolate(0), 3, ERROR);
    ASSERT_NEAR(spline.interpolate(0.5), 3.625, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}