#include <array>

/**
 * xType - тип аргумента x.
 * yType - тип значения функции y
 * N - количество точек для интерполяции
 *
 * Рекомедую обратить внимание. Разность (xType - xType) не обязана быть типом
 * xType
 */
template <typename xType, typename yType, unsigned int N>
class NewtonInterpolator {
    std::array<yType, N> m_divided_differences;
    const std::array<xType, N> m_points;

public:
    NewtonInterpolator(const std::array<xType, N> &points,
                       const std::array<yType, N> &values) noexcept
        : m_points{points}, m_divided_differences{values} {
        for (unsigned int j = 0; j < N; ++j) {
            for (unsigned int i = N - 1; i > j; --i) {
                m_divided_differences[i] =
                    (m_divided_differences[i] - m_divided_differences[i - 1]) /
                    (m_points[i] - m_points[i - j - 1]);
            }
        }
    }

    yType interpolate(const xType &x) const noexcept {
        yType res = m_divided_differences[N - 1];
        for (int i = N - 2; i >= 0; --i) {
            res = m_divided_differences[i] + (x - m_points[i]) * res;
        }
        return res;
    }
};

/**
 * xType - тип аргумента x.
 * yType - тип значения функции y
 * N - количество точек
 *
 * Рекомедую обратить внимание. Разность (xType - xType) не обязана быть типом
 * xType
 */
template <typename xType, typename yType, unsigned int N>
class HermiteInterpolator {
    std::array<yType, 2 * N> m_divided_differences;
    std::array<xType, 2 * N> m_points;

public:
    HermiteInterpolator(const std::array<xType, N> &points,
                        const std::array<yType, N> &values,
                        const std::array<yType, N> &deriv) noexcept {
        for (unsigned int i = 0; i < N; ++i) {
            m_points[2 * i] = points[i];
            m_points[2 * i + 1] = points[i];
            m_divided_differences[2 * i] = values[i];
            m_divided_differences[2 * i + 1] = values[i];
        }
        for (unsigned int j = 0; j < 2 * N; ++j) {
            for (int i = 2 * N - 1; i > j; --i) {
                if (m_points[i] == m_points[i - j - 1]) {
                    m_divided_differences[i] = deriv[i / 2];
                } else {
                    m_divided_differences[i] =
                        (m_divided_differences[i] -
                         m_divided_differences[i - 1]) /
                        (m_points[i] - m_points[i - j - 1]);
                }
            }
        }
    };

    yType interpolate(const xType &x) const noexcept {
        yType res = m_divided_differences[2 * N - 1];
        for (int i = 2 * N - 2; i >= 0; --i) {
            res = m_divided_differences[i] + (x - m_points[i]) * res;
        }
        return res;
    }
};
