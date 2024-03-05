#include <array>
#include <fstream>

template <unsigned int NX> struct Grid1D {
    const double X_LEFT;
    const double X_RIGHT;
    const double DX;
    std::array<double, NX> points;

    Grid1D(const double x_left, const double x_right)
        : X_LEFT(x_left), X_RIGHT(x_right), DX((X_RIGHT - X_LEFT) / (NX - 1)) {}
};

template <unsigned int NX> class ConvEqSolver {
protected:
    const double CFL;
    Grid1D<NX> grid;

    virtual void update_grid() = 0;

    void write_grid_to_file(std::ofstream &file) const {
        for (std::size_t i = 0; i < NX; ++i) {
            file << grid.X_LEFT + grid.DX * i << " " << grid.points[i]
                 << std::endl;
        }
    }

public:
    ConvEqSolver(const double cfl, const double x_left, const double x_right)
        : CFL(cfl), grid(x_left, x_right) {}

    ConvEqSolver(double cfl, const Grid1D<NX> &grid) : CFL(cfl), grid(grid) {}

    void solve(const double t_start, const double t_end, std::ofstream &file) {
        write_grid_to_file(file);
        file << std::endl << std::endl;
        for (double t = t_start; t < t_end; t += CFL * grid.DX) {
            update_grid();
            write_grid_to_file(file);
            file << std::endl << std::endl;
        }
    }
};

template <unsigned int NX>
class ConvEqSolverLeftCorner : public ConvEqSolver<NX> {
    using ConvEqSolver<NX>::CFL;
    using ConvEqSolver<NX>::grid;

public:
    ConvEqSolverLeftCorner(const double cfl, const double x_left,
                           const double x_right)
        : ConvEqSolver<NX>(cfl, x_left, x_right) {}

    ConvEqSolverLeftCorner(double cfl, const Grid1D<NX> &grid)
        : ConvEqSolver<NX>(cfl, grid) {}

    void update_grid() override {
        std::array<double, NX> points_new;
        for (std::size_t x = 1; x < NX; ++x) {
            points_new[x] =
                grid.points[x] - CFL * (grid.points[x] - grid.points[x - 1]);
        }
        points_new[0] = points_new[NX - 1];
        grid.points = points_new;
    }
};

template <unsigned int NX>
class ConvEqSolverLaxWendroff : public ConvEqSolver<NX> {
    using ConvEqSolver<NX>::CFL;
    using ConvEqSolver<NX>::grid;

public:
    ConvEqSolverLaxWendroff(const double cfl, const double x_left,
                            const double x_right)
        : ConvEqSolver<NX>(cfl, x_left, x_right) {}

    ConvEqSolverLaxWendroff(double cfl, const Grid1D<NX> &grid)
        : ConvEqSolver<NX>(cfl, grid) {}

    void update_grid() override {
        std::array<double, NX> points_new;
        for (std::size_t x = 1; x < NX - 1; ++x) {
            points_new[x] =
                grid.points[x] -
                CFL * (grid.points[x + 1] - grid.points[x - 1]) / 2 +
                CFL * CFL *
                    (grid.points[x + 1] - 2 * grid.points[x] +
                     grid.points[x - 1]) /
                    2;
        }
        points_new[NX - 1] = grid.points[NX - 1] -
                             CFL / 2 * (grid.points[1] - grid.points[NX - 2]) +
                             CFL * CFL / 2 *
                                 (grid.points[1] - 2 * grid.points[NX - 1] +
                                  grid.points[NX - 2]);
        points_new[0] = points_new[NX - 1];
        grid.points = points_new;
    }
};
