#include "nonlinear_equations.h"
#include <fstream>
#include <iomanip>
#include <vector>

int main() {
    std::ofstream data;
    data << std::setprecision(16);
    data.open("e01.txt");
    for (unsigned int i = 0; i < 100; ++i) {
        data << i << " " << keplerSolver(0.1, M_PI / 4., i, -1) << std::endl;
    }
    data.close();

    data.open("e02.txt");
    for (unsigned int i = 0; i < 100; ++i) {
        data << i << " " << keplerSolver(0.2, M_PI / 4., i, -1) << std::endl;
    }
    data.close();

    data.open("e05.txt");
    for (unsigned int i = 0; i < 100; ++i) {
        data << i << " " << keplerSolver(0.5, M_PI / 4., i, -1) << std::endl;
    }
    data.close();

    data.open("e08.txt");
    for (unsigned int i = 0; i < 100; ++i) {
        data << i << " " << keplerSolver(0.8, M_PI / 4., i, -1) << std::endl;
    }
    data.close();
}