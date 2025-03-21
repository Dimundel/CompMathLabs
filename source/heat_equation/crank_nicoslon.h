#include "tridiagonal_matrix.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#ifndef CRANK_NICOLSON
#define CRANK_NICOLSON

template <typename T, unsigned int NX> struct Grid1D {
    const double X_LEFT;
    const double X_RIGHT;
    const double DX;
    std::array<T, NX> points;

    Grid1D(const double x_left, const double x_right)
        : X_LEFT(x_left), X_RIGHT(x_right), DX((X_RIGHT - X_LEFT) / (NX - 1)) {}

    T &operator[](std::size_t index) { return points[index]; }

    const T &operator[](std::size_t index) const { return points[index]; }

    friend std::ostream &operator<<(std::ostream &os,
                                    const Grid1D<T, NX> &grid) {
        for (std::size_t i = 0; i < NX; ++i) {
            double x = grid.X_LEFT + i * grid.DX;
            os << x << " " << grid[i] << std::endl;
        }
        return os;
    }
};

struct HeatEquation {
    double coef;
    std::function<double(double, double)> func;
    std::array<double, 2> left_boundary_conditions;
    std::function<double(double)> left_func;
    std::array<double, 2> right_boundary_conditions;
    std::function<double(double)> right_func;
};

template <typename T, unsigned int NX>
void solve(const HeatEquation &eq, const Grid1D<T, NX> &grid,
           const double time_end, const double dT, std::ostream &file) {
    std::ofstream os;
    Grid1D<T, NX> curr_grid = grid;
    Grid1D<T, NX> next_grid = grid;
    double curr_time = 0;
    double Co = eq.coef * dT / grid.DX / grid.DX / 2.;
    std::array<std::array<T, 3>, NX> data;
    for (int i = 1; i < NX - 1; ++i) {
        data[i][0] = -Co;
        data[i][1] = 1 + 2 * Co;
        data[i][2] = -Co;
    }
    data[0][0] = 0;
    data[0][1] = eq.left_boundary_conditions[0] -
                 eq.left_boundary_conditions[1] / grid.DX;
    data[0][2] = eq.left_boundary_conditions[1] / grid.DX -
                 eq.left_boundary_conditions[1] / 2. / grid.DX / Co;
    data[NX - 1][0] = -eq.right_boundary_conditions[1] / grid.DX +
                      eq.right_boundary_conditions[1] / 2. / grid.DX / Co;
    data[NX - 1][1] = eq.right_boundary_conditions[0] +
                      eq.right_boundary_conditions[1] / grid.DX;
    data[NX - 1][2] = 0;
    TridiagonalMatrix<T, NX> matrix(data);
    std::array<double, NX> column;

    file << curr_time << std::endl;
    file << curr_grid << std::endl << std::endl;

    while (curr_time < time_end) {
        column[0] = eq.left_func(curr_time + dT) -
                    (curr_grid[1] +
                     Co * (curr_grid[0] - 2 * curr_grid[1] + curr_grid[2]) +
                     (eq.func(curr_time, grid.X_LEFT + grid.DX) +
                      eq.func(curr_time + dT, grid.X_LEFT + grid.DX)) *
                         dT / 2) *
                        (eq.left_boundary_conditions[1] / Co / 2. / grid.DX);
        for (int i = 1; i < NX - 1; ++i) {
            column[i] =
                curr_grid[i] +
                Co * (curr_grid[i - 1] - 2 * curr_grid[i] + curr_grid[i + 1]) +
                (eq.func(curr_time, grid.X_LEFT + i * grid.DX) +
                 eq.func(curr_time + dT, grid.X_LEFT + i * grid.DX)) *
                    dT / 2;
        }
        column[NX - 1] =
            eq.right_func(curr_time + dT) +
            (curr_grid[NX - 2] +
             Co * (curr_grid[NX - 1] - 2 * curr_grid[NX - 2] +
                   curr_grid[NX - 3]) +
             (eq.func(curr_time, grid.X_RIGHT - grid.DX) +
              eq.func(curr_time + dT, grid.X_RIGHT - grid.DX)) *
                 dT / 2) *
                (eq.right_boundary_conditions[1] / Co / 2. / grid.DX);
        next_grid.points = matrix.solve(column);
        file << curr_time + dT << std::endl;
        file << next_grid << std::endl << std::endl;
        curr_grid.points = next_grid.points;
        curr_time += dT;
    }
}

#endif