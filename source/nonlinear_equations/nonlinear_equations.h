#include <cmath>

/**
    Решает уравнение Кеплера методом Ньютона
    * ecc - эксцентриситет, принадлежит (0, 1)
    * meanAnomaly - средняя аномалия, М (радианы)
    * maxIter - максимальное количество итераций
    * tol - точность, с которой нужно отыскать решение

    Рекомендуемое поведение. Если решение не нашлось за maxIter итераций -
выбрасывать исключение. Если приближения к решению между итерациями меняются не
более, чем на tol, то решение достигнуто.
**/
double keplerSolver(double ecc, double meanAnomaly, unsigned int maxIter,
                    double tol) {
    double res1 = meanAnomaly;
    double res2 = res1 - (res1 - ecc * std::sin(res1) - meanAnomaly) /
                             (1 - ecc * std::cos(res1));
    for (unsigned int i = 0; i < maxIter and std::abs(res1 - res2) > tol; ++i) {
        res1 = res2;
        res2 = res1 - (res1 - ecc * std::sin(res1) - meanAnomaly) /
                          (1 - ecc * std::cos(res1));
    }
    return res2;
}

template <typename A> struct ArgumentGetter;

template <typename R, typename Arg> struct ArgumentGetter<R(Arg)> {
    using Argument = Arg;
};

template <typename Callable, typename RealType>
decltype(auto) solve(const Callable &func, // функция F
                     const RealType &tau,  // шаг тау
                     const typename ArgumentGetter<Callable>::Argument
                         &initialGuess, // начальное приближение
                     const unsigned int nIteration // количество итераций
) {
    typename ArgumentGetter<Callable>::Argument res = initialGuess;
    for (unsigned int i = 0; i < nIteration; ++i) {
        res += tau * func(res);
    }
    return res;
}