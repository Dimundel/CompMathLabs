#include <array>
#include <numeric>
#include <type_traits>

template <typename A> struct ArgumentGetter;

template <typename R, typename Arg> struct ArgumentGetter<R(Arg)> {
    using Argument = Arg;
};

template <typename RealType, std::size_t N> struct Nodes;

template <typename RealType> struct Nodes<RealType, 1> {
    static constexpr std::array<RealType, 1> nodes{0};
};

template <typename RealType> struct Nodes<RealType, 2> {
    static constexpr std::array<RealType, 2> nodes{-0.5773502691896257,
                                                   0.5773502691896257};
};

template <typename RealType> struct Nodes<RealType, 3> {
    static constexpr std::array<RealType, 3> nodes{
        -0.7745966692414834, 0.0000000000000000, 0.7745966692414834};
};

template <typename RealType> struct Nodes<RealType, 4> {
    static constexpr std::array<RealType, 4> nodes{
        -0.8611363115940526, -0.3399810435848563, 0.3399810435848563,
        0.8611363115940526};
};

template <typename RealType> struct Nodes<RealType, 5> {
    static constexpr std::array<RealType, 5> nodes{
        -0.9061798459386640, -0.5384693101056831, 0, 0.5384693101056831,
        0.9061798459386640};
};

template <typename RealType, std::size_t N> struct Weights;

template <typename RealType> struct Weights<RealType, 1> {
    static constexpr std::array<RealType, 1> weights{2};
};

template <typename RealType> struct Weights<RealType, 2> {
    static constexpr std::array<RealType, 2> weights{1, 1};
};

template <typename RealType> struct Weights<RealType, 3> {
    static constexpr std::array<RealType, 3> weights{
        0.5555555555555556, 0.8888888888888888, 0.5555555555555556};
};

template <typename RealType> struct Weights<RealType, 4> {
    static constexpr std::array<RealType, 4> weights{
        0.3478548451374538, 0.6521451548625461, 0.6521451548625461,
        0.3478548451374538};
};

template <typename RealType> struct Weights<RealType, 5> {
    static constexpr std::array<RealType, 5> weights{
        0.2369268850561891, 0.4786286704993665, 0.5688888888888889,
        0.4786286704993665, 0.2369268850561891};
};

template <typename T>
using Dif = decltype(std::declval<T>() - std::declval<T>());

/* Функция производит интегрирование на одном отрезке */
template <typename Callable, typename RealType, std::size_t N>
decltype(auto) integrate(
    const Callable &func, // Интегрируемая функция
    const typename ArgumentGetter<Callable>::Argument &start, // начало отрезка
    const typename ArgumentGetter<Callable>::Argument &end // конец отрезка
) {
    RealType semi_sum = (start + end) / 2;
    RealType semi_dif = (end - start) / 2;
    RealType temp_sum = 0;
    for (unsigned int i = 0; i < N; ++i) {
        temp_sum += Weights<RealType, N>::weights[i] *
                    func(semi_dif * Nodes<RealType, N>::nodes[i] + semi_sum);
    }
    return semi_dif * temp_sum;
}

/* Функция производит интегрирование, разбивая отрезок на подотрезки длиной
 * не более dx */
template <typename Callable, typename RealType, std::size_t N>
decltype(auto) integrate(
    const Callable &func, // Интегрируемая функция
    const typename ArgumentGetter<Callable>::Argument &start, // начало отрезка
    const typename ArgumentGetter<Callable>::Argument &end, // конец отрезка
    const Dif<typename ArgumentGetter<Callable>::Argument>
        &dx // Длина подотрезка
) {
    const int num = (end - start) / dx + 1;
    Dif<typename ArgumentGetter<Callable>::Argument> delta =
        (end - start) / num;
    decltype(auto) res =
        integrate<Callable, RealType, N>(func, start, start + delta);
    for (unsigned int i = 1; i < num; ++i) {
        res += integrate<Callable, RealType, N>(func, start + i * delta,
                                                start + (i + 1) * delta);
    }
    return res;
}