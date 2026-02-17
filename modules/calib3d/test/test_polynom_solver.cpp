// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"
#include "../src/polynom_solver.h"

namespace opencv_test { namespace {

// Helper function to verify roots by plugging them back into the equation
static void checkRoots2(double a, double b, double c, int expected_n)
{
    double x1, x2;
    int n = solve_deg2(a, b, c, x1, x2);

    ASSERT_EQ(expected_n, n) << "Number of roots mismatch for a=" << a << " b=" << b << " c=" << c;

    for (int i = 0; i < n; i++)
    {
        double x = (i == 0) ? x1 : x2;

        // Calculate residual: |ax^2 + bx + c|
        double res = std::abs(a * x * x + b * x + c);

        // Normalize by the magnitude of terms to handle large numbers (relative error)
        double scale = std::max({std::abs(a * x * x), std::abs(b * x), std::abs(c)});
        if (scale < 1e-30) scale = 1.0; // Prevent division by zero for trivial zero roots

        EXPECT_LT(res / scale, 1e-14) << "Residual too high for a=" << a << " b=" << b << " c=" << c << " x=" << x;
    }
}

TEST(Calib3d_PolynomSolver, solve_deg2_basic)
{
    // Distinct real roots: x^2 - 5x + 6 = 0 -> (x-2)(x-3)
    checkRoots2(1, -5, 6, 2);

    // No real roots: x^2 + 0x + 1 = 0
    double tmp1, tmp2;
    ASSERT_EQ(0, solve_deg2(1, 0, 1, tmp1, tmp2));
}

TEST(Calib3d_PolynomSolver, solve_deg2_edge_cases)
{
    // Double root: x^2 - 2x + 1 = 0 -> (x-1)^2
    // solve_deg2 returns 1 for double roots
    checkRoots2(1, -2, 1, 1);

    // Pure quadratic: x^2 - 4 = 0 -> x = +/- 2
    checkRoots2(1, 0, -4, 2);
}

TEST(Calib3d_PolynomSolver, solve_deg2_vieta_stability)
{
    // Catastrophic cancellation cases (High 'b', small 'a', 'c')
    // This verifies the fix for numerical stability
    checkRoots2(1,  1e8,  1, 2);
    checkRoots2(1, -1e8,  1, 2);
    checkRoots2(1,  1e15, 1, 2);
}

TEST(Calib3d_PolynomSolver, solve_deg3_roots)
{
    // (x-1)(x-2)(x-3) = x^3 - 6x^2 + 11x - 6
    double x0, x1, x2;
    int n = solve_deg3(1, -6, 11, -6, x0, x1, x2);
    ASSERT_EQ(3, n);

    double roots[] = {x0, x1, x2};
    for (int i=0; i<3; ++i) {
        double x = roots[i];
        double val = ((x - 6) * x + 11) * x - 6;
        EXPECT_NEAR(0.0, val, 1e-10);
    }
}

TEST(Calib3d_PolynomSolver, solve_deg4_roots)
{
    // (x-1)(x-2)(x-3)(x-4) = x^4 - 10x^3 + 35x^2 - 50x + 24
    double x0, x1, x2, x3;
    int n = solve_deg4(1, -10, 35, -50, 24, x0, x1, x2, x3);
    ASSERT_EQ(4, n);

    double roots[] = {x0, x1, x2, x3};
    for (int i=0; i<4; ++i) {
        double x = roots[i];
        double val = (((x - 10) * x + 35) * x - 50) * x + 24;
        EXPECT_NEAR(0.0, val, 1e-8);
    }
}

}} // namespace opencv_test
