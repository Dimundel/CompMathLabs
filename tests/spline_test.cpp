#include "spline.h"
#include <gtest/gtest.h>

const double ERROR = 1e-3;

TEST(SplineTest, Interpolation) {
    std::vector<double> points{-1, 0, 1};
    std::vector<double> values{1, 2, 5};
    CubicSpline<double, double> spline(points, values);
    ASSERT_NEAR(spline.interpolate(0), 2, ERROR);
    ASSERT_NEAR(spline.interpolate(-1), 1, ERROR);
    ASSERT_NEAR(spline.interpolate(1), 5, ERROR);
    ASSERT_NEAR(spline.interpolate(0.5), 3.313, ERROR);
    ASSERT_NEAR(spline.interpolate(-0.5), 1.313, ERROR);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}