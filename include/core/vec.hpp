#pragma once

#include <cmath>
#include <core/int_types.hpp>
#include <cstring> // For std::memset

namespace radiant
{
// TODO: SIMD enabled vector?

template <typename T, size_t N>
class vec;

using vec2f = vec<f32, 2>;
using vec3f = vec<f32, 3>;
using vec4f = vec<f32, 4>;

using vec2d = vec<f64, 2>;
using vec3d = vec<f64, 3>;
using vec4d = vec<f64, 4>;

using vec2i = vec<i32, 2>;
using vec3i = vec<i32, 3>;
using vec4i = vec<i32, 4>;

using vec2ui = vec<u32, 2>;
using vec3ui = vec<u32, 3>;
using vec4ui = vec<u32, 4>;

template <typename T, size_t N>
struct vec
{
    T m_data[N];

    vec() { std::memset(m_data, 0, N); }
    vec(const T* data) { std::memcpy(m_data, data, sizeof(T) * N); }

    template<typename... Args>
    constexpr vec(Args&&... args) : m_data{static_cast<T>(std::forward<Args>(args))...} {
        static_assert(sizeof...(Args) == N, "Number of arguments must match vector size");
    }
    const T& operator[](u32 i) const { return m_data[i]; };
    T&       operator[](u32 i) { return m_data[i]; };

    vec operator-() const
    {
        vec result(m_data);
        for (u32 i = 0; i < N; ++i)
        {
            result.m_data[i] *= -1;
        }
        return result;
    }

    vec& operator+=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            m_data[i] += v.m_data[i];
        }
        return *this;
    }

    vec& operator-=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            m_data[i] -= v.m_data[i];
        }
        return *this;
    }

    vec& operator*=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            m_data[i] *= v.m_data[i];
        }
        return *this;
    }

    vec& operator/=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            m_data[i] /= v.m_data[i];
        }
        return *this;
    }

    vec& operator*=(T scalar)
    {
        for (u32 i = 0; i < N; ++i)
        {
            m_data[i] *= scalar;
        }
        return *this;
    }

    vec& operator/=(T scalar)
    {
        for (u32 i = 0; i < N; ++i)
        {
            m_data[i] /= scalar;
        }
        return *this;
    }

    f32 dot(const vec& v) const
    {
        f32 result = 0.0f;
        for (u32 i = 0; i < N; ++i)
        {
            result += m_data[i] * v.m_data[i];
        }
        return result;
    }

    vec& normalize() { return *this / this->length(); }

    f32 length() const { return std::sqrt(dot(*this)); }
    f32 sqlength() const { return dot(*this); }
};

template <typename T, size_t N>
inline vec<T, N> operator+(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] + b[i];
    }
    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator-(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] - b[i];
    }
    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator*(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] * b[i];
    }
    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator/(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] / b[i];
    }
    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator*(const vec<T, N>& a, f32 t)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] * t;
    }
    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator*(f32 t, const vec<T, N>& a)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] * t;
    }
    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator/(const vec<T, N>& a, f32 t)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] / t;
    }
    return result;
}

template <typename T, size_t N>
inline vec<T, N> dot(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] * b[i];
    }
    return result;
}

template <typename T>
inline vec<T, 3> cross(const vec<T, 3>& a, const vec<T, 3>& b)
{
    vec<T, 3> result;
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
    return result;
}

template <typename T, size_t N>
inline vec<T, N> normalized(const vec<T, N>& v)
{
    return v / v.length();
}
} // namespace radiant