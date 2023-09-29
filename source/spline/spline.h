#include <array>
#include <type_traits>
#include <vector>

/** класс для работы с трехдиагональной матрицей **/
template <typename Type> class ThreeDiagonalMatrix {
    /*** Здесь какие-то поля и методы ***/
private:
    std::vector<std::array<Type, 3>> m_data;

public:
    ThreeDiagonalMatrix(const std::vector<std::array<Type, 3>> &data)
        : m_data{data} {}
    Type operator()(const unsigned int i, const unsigned int j) const {
        if (j == i - 1) {
            return m_data[i][0];
        } else if (j == i) {
            return m_data[i][1];
        } else if (j == i + 1) {
            return m_data[i][2];
        } else {
            return 0;
        }
    }
};

template <typename numeratorType, typename denominatorType>
using DivisType =
    decltype(std::declval<numeratorType>() / std::declval<denominatorType>());

/** Функция для решения методм  прогонки **/
template <typename mType, typename cType>
std::vector<DivisType<cType, mType>>
solve(const ThreeDiagonalMatrix<mType> &matrix,
      const std::vector<cType> &column) {
    std::vector<double> p_vector{0}, q_vector{0};
    unsigned int N = column.size() - 1;
    p_vector.reserve(N);
    q_vector.reserve(N);
    for (int i = 0; i < N + 1; ++i) {
        p_vector.push_back(-1 * matrix(i, i + 1) /
                           (matrix(i, i - 1) * p_vector[i] + matrix(i, i)));
        q_vector.push_back((column[i] - matrix(i, i - 1) * q_vector[i]) /
                           (matrix(i, i - 1) * p_vector[i] + matrix(i, i)));
    }
    std::vector<double> solution(N + 1);
    solution[N] = (column[N] - matrix(N, N - 1) * q_vector[N]) /
                  (matrix(N, N - 1) * p_vector[N] + matrix(N, N));
    for (int i = N - 1; i >= 0; --i) {
        solution[i] = p_vector[i + 1] * solution[i + 1] + q_vector[i + 1];
    }
    return solution;
}

/**
 * xType - тип аргумента x.
 * yType - тип значения функции y
 */
template <typename xType, typename yType> class CubicSpline {
    /*** Какие-то поля ***/

public:
    CubicSpline(const std::vector<xType> &points, // Значения x
                const std::vector<yType> &values  // значения y
    );

    yType interpolate(const xType &x) const noexcept;
};