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
    p_vector.reserve(matrix.get_size());
    q_vector.reserve(matrix.get_size());
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