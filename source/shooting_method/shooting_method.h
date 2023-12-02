#include "../runge_kutta/runge_kutta.h"
#include <vector>

template <typename RHS, typename RealType>
double solve(const std::function<double(const double &)> func,
             const RealType &epsilon,
             const RealType &tau,     // шаг тау
             const double &initGuess, // начальное приближение
             const unsigned int nIteration // количество итераций
) {
    double res1 = initGuess;
    double res2;
    for (unsigned int i = 0; i < nIteration; ++i) {
        res2 += tau * func(res1);
        if (std::abs(res2 - res1) < epsilon) {
            break;
        }
        res1 = res2;
    }
    return res2;
}

struct IntegrationParameters {
    double step; // шаг интегрирования
    double epsilon; // точность решения нелинейного уравнения
    double tau; // параметр тау для решения нелинейного уравнения
    unsigned int maxIter; // максимальное количество итераций для
                          // решения нелинейного уравнения
};

template <typename Table, typename RHS>
std::vector<typename RHS::StateAndArg>
integrate(const std::array<double, 2> &initialState,
          const typename std::array<double, 2> &finalState,
          const double initGuess, const IntegrationParameters &parameters,
          const RHS &rhs) {
    std::vector<typename RHS::StateAndArg> res;
    auto func = [initialState, finalState, parameters,
                 rhs](const double v) {
        return RungeKutta::integrate<Table, RHS>(
                   {{initialState[1], v}, initialState[0]}, finalState[0],
                   parameters.step, rhs)
                   .back()
                   .state[0] -
               finalState[1];
    };
    double rightGuess =
        solve<RHS, double>(func, parameters.epsilon, parameters.tau, initGuess,
                           parameters.maxIter);
    return RungeKutta::integrate<Table, RHS>(
        {{initialState[1], rightGuess}, initialState[0]}, finalState[0],
        parameters.step, rhs);
}
