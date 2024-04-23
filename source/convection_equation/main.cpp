#include "ConvEqSolver.h"
#include <cmath>

// const double CFL = 1;
const double H = 0.5;
const double X_LEFT = 0;
const double X_RIGHT = 20;
const unsigned int NX = (X_RIGHT - X_LEFT) / H + 1;

double left_corner(const double m1, const double m2, const double CFL) {
    return 0;
}

int main() {
    Grid1D<NX> grid(X_LEFT, X_RIGHT);
    for (std::size_t i = 0; i < NX; ++i) {
        grid.points[i] =
            std::sin(4 * M_PI * (X_LEFT + H * i) / (X_RIGHT - X_LEFT));
    }

    std::ofstream file;
    ConvEqSolverLeftCorner<NX> solver1(1, grid);
    ConvEqSolverLeftCorner<NX> solver2(1.01, grid);
    ConvEqSolverLeftCorner<NX> solver3(0.6, grid);
    file.open("data_lc1.txt");
    solver1.solve(0, 20, file);
    file.close();
    file.open("data_lc101.txt");
    solver2.solve(0, 20, file);
    file.close();
    file.open("data_lc06.txt");
    solver3.solve(0, 20, file);
    file.close();

    ConvEqSolverLaxWendroff<NX> solver4(1, grid);
    ConvEqSolverLaxWendroff<NX> solver5(1.1, grid);
    ConvEqSolverLaxWendroff<NX> solver6(0.6, grid);
    file.open("data_lw1.txt");
    solver4.solve(0, 20, file);
    file.close();
    file.open("data_lw101.txt");
    solver5.solve(0, 70, file);
    file.close();
    file.open("data_lw06.txt");
    solver6.solve(0, 20, file);
    file.close();
}