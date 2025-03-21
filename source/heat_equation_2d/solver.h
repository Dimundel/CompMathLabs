#include "csr_matrix.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

template <typename T, unsigned int NX, unsigned int NY> struct Grid2D {
    const double X_LEFT;
    const double X_RIGHT;
    const double DX;
    const double Y_BOTTOM;
    const double Y_TOP;
    const double DY;
    std::array<T, NX * NY> points;

    Grid2D(const double x_left, const double x_right, const double y_bottom,
           const double y_top)
        : X_LEFT(x_left), X_RIGHT(x_right), DX((X_RIGHT - X_LEFT) / (NX - 1)),
          Y_BOTTOM(y_bottom), Y_TOP(y_top), DY((Y_TOP - Y_BOTTOM) / (NY - 1)) {}

    T &operator()(unsigned int row, unsigned int col) {
        assert(row < NY && col < NX);
        return points[row * NX + col];
    }

    const T &operator()(unsigned int row, unsigned int col) const {
        assert(row < NY && col < NX);
        return points[row * NX + col];
    }

    void print() const {
        for (std::size_t row = 0; row < NY; ++row) {
            for (std::size_t col = 0; col < NX; ++col) {
                std::cout << std::setw(10) << (*this)(row, col) << " ";
            }
            std::cout << std::endl;
        }
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const Grid2D<T, NX, NY> &grid) {
        for (std::size_t j = 0; j < NY; ++j) {
            double y = grid.Y_BOTTOM + j * grid.DY;
            for (std::size_t i = 0; i < NX; ++i) {
                double x = grid.X_LEFT + i * grid.DX;
                os << x << " " << y << " " << grid(j, i) << std::endl;
            }
        }
        return os;
    }
};

template <typename T, unsigned int NX, unsigned int NY>
std::vector<T>
toRowMajorVectorSkippingBoundaries(const Grid2D<T, NX, NY> &grid) {
    if (NX < 2 || NY < 2) {
        return {};
    }

    std::vector<T> result;
    result.reserve((NX - 2) * (NY - 2));

    for (unsigned int row = 1; row < NY - 1; ++row) {
        for (unsigned int col = 1; col < NX - 1; ++col) {
            result.push_back(grid(row, col));
        }
    }

    return result;
}

template <typename T, unsigned int NX, unsigned int NY> struct HeatEquation2D {
    double coef;
    std::function<double(double, double, double)> func;
};

template <typename T, unsigned int NX, unsigned int NY>
void solve(const HeatEquation2D<T, NX, NY> &eq, const Grid2D<T, NX, NY> &grid,
           const double time_end, const double dT, std::ostream &file) {
    Grid2D<T, NX, NY> curr_grid = grid;
    double curr_time = 0;
    double Co_x = dT * eq.coef / grid.DX / grid.DX;
    double Co_y = dT * eq.coef / grid.DY / grid.DY;
    std::cout << "Co_x: " << Co_x << "  Co_y: " << Co_y << std::endl;
    std::map<std::pair<int, int>, T> map;
    for (int i = 0; i < (NX - 2) * (NY - 2); ++i) {
        map.insert({{i, i}, 1 + 2 * (Co_x + Co_y)});
    }
    for (int i = 0; i < NY - 2; ++i) {
        for (int j = 0; j < NX - 2 - 1; ++j) {
            map.insert({{i * (NX - 2) + j, i * (NX - 2) + j + 1}, -Co_x});
            map.insert({{i * (NX - 2) + j + 1, i * (NX - 2) + j}, -Co_x});
        }
    }
    for (int i = 0; i < (NX - 2) * (NY - 2) - (NX - 2); ++i) {
        map.insert({{i, i + (NX - 2)}, -Co_y});
        map.insert({{i + (NX - 2), i}, -Co_y});
    }
    CSRMatrix<T> matrix(map);

    std::vector<T> column((NX - 2) * (NY - 2));

    file << curr_time << std::endl;
    file << curr_grid << std::endl << std::endl;

    // matrix.print();

    while (curr_time < time_end) {
        for (int i = 1; i < NY - 3; ++i) {
            // left
            column[(NX - 2) * i] =
                curr_grid(i + 1, 1) + Co_x * curr_grid(i + 1, 0) +
                dT * eq.func(curr_time + dT, grid.X_LEFT + 1 * grid.DX,
                             grid.Y_BOTTOM + (i + 1) * grid.DY);
            // right
            column[(NX - 2) * (i + 1) - 1] =
                curr_grid(i + 1, NX - 2) + Co_x * curr_grid(i + 1, NX - 1) +
                dT * eq.func(curr_time + dT, grid.X_RIGHT - grid.DX,
                             grid.Y_BOTTOM + (i + 1) * grid.DY);
        }
        for (int i = 1; i < NX - 3; ++i) {
            // bottom
            column[i] =
                curr_grid(1, i + 1) + Co_y * curr_grid(0, i + 1) +
                dT * eq.func(curr_time + dT, grid.X_LEFT + (i + 1) * grid.DX,
                             grid.Y_BOTTOM + grid.DY);
            // top
            column[(NX - 2) * (NY - 2) - (NX - 2) + i] =
                curr_grid(NY - 2, i + 1) + Co_y * curr_grid(NY - 1, i + 1) +
                dT * eq.func(curr_time + dT, grid.X_LEFT + (i + 1) * grid.DX,
                             grid.Y_TOP - grid.DY);
        }
        // center
        for (int i = 1; i < NY - 3; ++i) {
            for (int j = 1; j < NX - 3; ++j) {
                column[i * (NX - 2) + j] =
                    curr_grid(i + 1, j + 1) +
                    dT * eq.func(curr_time + dT,
                                 grid.X_LEFT + (j + 1) * grid.DX,
                                 grid.Y_BOTTOM + (i + 1) * grid.DY);
            }
        }
        // bottom left corner
        column[0] = curr_grid(1, 1) + Co_x * curr_grid(1, 0) +
                    Co_y * curr_grid(0, 1) +
                    dT * eq.func(curr_time + dT, grid.X_LEFT + grid.DX,
                                 grid.Y_BOTTOM + grid.DY);
        // bottom right corner
        column[NX - 3] = curr_grid(1, NX - 2) + Co_x * curr_grid(1, NX - 1) +
                         Co_y * curr_grid(0, NX - 2) +
                         dT * eq.func(curr_time + dT, grid.X_RIGHT - grid.DX,
                                      grid.Y_BOTTOM + grid.DY);
        // top left corner
        column[(NX - 2) * (NY - 3)] =
            curr_grid(NY - 2, 1) + Co_x * curr_grid(NY - 2, 0) +
            Co_y * curr_grid(NY - 1, 1) +
            dT * eq.func(curr_time + dT, grid.X_LEFT + grid.DX,
                         grid.Y_TOP - grid.DY);
        // top right corner
        assert((NX - 2) * (NY - 2) - 1 < column.size());
        column[(NX - 2) * (NY - 2) - 1] =
            curr_grid(NY - 2, NX - 2) + Co_x * curr_grid(NY - 2, NX - 1) +
            Co_y * curr_grid(NY - 1, NX - 2) +
            dT * eq.func(curr_time + dT, grid.X_RIGHT - grid.DX,
                         grid.Y_TOP - grid.DY);

        std::vector<T> res = conjugate_gradients<T>(
            matrix, column,
            toRowMajorVectorSkippingBoundaries<T, NX, NY>(curr_grid), 1e-50);
        for (int i = 1; i < NY - 1; ++i) {
            for (int j = 1; j < NX - 1; ++j) {
                curr_grid(i, j) = res[(NX - 2) * (i - 1) + (j - 1)];
            }
        }

        file << curr_time + dT << std::endl;
        file << curr_grid << std::endl << std::endl;
        curr_time += dT;
    }
}