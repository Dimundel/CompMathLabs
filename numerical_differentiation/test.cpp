#include <array>
#include <eigen3/Eigen/Dense>
#include <iostream>

int factorial(const int n) {
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

int main() {
    std::cout << log(1e-15);

}