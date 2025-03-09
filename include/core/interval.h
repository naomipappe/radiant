#pragma once

#include <limits>
namespace radiant
{
template <class T>
struct Interval
{
    T m_min, m_max;

    Interval() : m_min(std::numeric_limits<T>::infinity()), m_max(-std::numeric_limits<T>::infinity()) {}
    Interval(T min, T max) : m_min(min), m_max(max) {}

    T    length() const { return m_max - m_min; }
    bool contains(T value) const { return value >= m_min && value <= m_max; }
    bool surrounds(T value) const { return value > m_min && value < m_max; }

    static const Interval<T> empty{};
    static const Interval<T> universe{ -empty };
};

} // namespace radiant