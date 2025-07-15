#pragma once

#include <algorithm>
#include <cmath>
#include <core/types.hpp>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace radiant
{
// TODO: SIMD enabled vector?

template <typename T, size_t N>
struct vec;

using vec2 = vec<Scalar, 2>;
using vec3 = vec<Scalar, 3>;
using vec4 = vec<Scalar, 4>;

using vec2f = vec<Scalar, 2>;
using vec3f = vec<Scalar, 3>;
using vec4f = vec<Scalar, 4>;

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

    vec() : m_data{} {}
    explicit vec(const T* data) { std::copy(data, data + N, m_data); }
    explicit vec(T* data) { std::copy(data, data + N, m_data); }
    explicit vec(const T value) { std::fill(m_data, m_data + N, value); }

    template <typename... Args>
    constexpr explicit vec(Args&&... args) : m_data{ static_cast<T>(std::forward<Args>(args))... }
    {
        static_assert(sizeof...(Args) == N, "Number of arguments must match vector size");
    }
    const T& operator[](u32 i) const { return m_data[i]; };
    T&       operator[](u32 i) { return m_data[i]; };

    bool is_zero() const
    {
        bool result = true;
        for (u32 i = 0; i < N; ++i)
        {
            result = result && (std::fabs(m_data[i]) < std::numeric_limits<Scalar>::epsilon());
        }
        return result;
    }

    vec operator-() const
    {
        vec result;
        for (u32 i = 0; i < N; ++i)
        {
            result.m_data[i] = -m_data[i]; // Proper negation
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

    vec& operator+=(T scalar)
    {
        for (u32 i = 0; i < N; ++i)
        {
            m_data[i] += scalar;
        }
        return *this;
    }

    T dot(const vec& v) const
    {
        T result = 0.0f;
        for (u32 i = 0; i < N; ++i)
        {
            result += m_data[i] * v.m_data[i];
        }
        return result;
    }

    vec& normalize()
    {
        *this /= this->length();
        return *this;
    }

    Scalar length() const { return std::sqrt(dot(*this)); }
    Scalar length_squared() const { return dot(*this); }
};

template <typename T, size_t N>
vec<T, N> operator+(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] + b[i];
    }
    return result;
}

template <typename T, size_t N>
vec<T, N> operator+(const vec<T, N>& a, T b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] + b;
    }
    return result;
}

template <typename T, size_t N>
vec<T, N> operator+(T b, const vec<T, N>& a)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] + b;
    }
    return result;
}

template <typename T, size_t N>
vec<T, N> operator-(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] - b[i];
    }
    return result;
}

template <typename T, size_t N>
vec<T, N> operator*(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] * b[i];
    }
    return result;
}

template <typename T, size_t N>
vec<T, N> operator/(const vec<T, N>& a, const vec<T, N>& b)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] / b[i];
    }
    return result;
}

template <typename V>
concept ScalarT = std::is_integral_v<V> || std::is_floating_point_v<V>;

template <typename T, ScalarT V, size_t N>
vec<T, N> operator*(const vec<T, N>& a, V t)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] * t;
    }
    return result;
}

template <typename T, ScalarT V, size_t N>
vec<T, N> operator*(V t, const vec<T, N>& a)
{
    return a * t;
}

template <typename T, typename V, size_t N>
vec<T, N> operator/(const vec<T, N>& a, V t)
{
    vec<T, N> result;
    for (u32 i = 0; i < N; ++i)
    {
        result[i] = a[i] / t;
    }
    return result;
}

template <typename T, size_t N>
T dot(const vec<T, N>& a, const vec<T, N>& b)
{
    T result{ 0 };
    for (u32 i = 0; i < N; ++i)
    {
        result += a[i] * b[i];
    }
    return result;
}

template <typename T>
vec<T, 3> cross(const vec<T, 3>& a, const vec<T, 3>& b)
{
    return vec<T, 3>{ a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] };
}

template <typename T, size_t N>
vec<T, N> normalized(const vec<T, N>& v)
{
    return v / v.length();
}

template <typename T>
vec<T, 3> reflect(const vec<T, 3>& v, const vec<T, 3>& n)
{
    return v - 2.0f * dot(v, n) * n;
}

template <typename T>
vec<T, 3> refract(const vec<T, 3>& v, const vec<T, 3>& n, Scalar effective_refraction_index)
{
    Scalar    cos_theta     = std::fmin(dot(-v, n), 1.0);
    vec<T, 3> perpendicular = effective_refraction_index * (v + cos_theta * n);
    vec<T, 3> parallel      = -std::sqrt(std::fabs(1 - perpendicular.length_squared())) * n;
    return perpendicular + parallel;
}

template <typename T, size_t N>
vec<T, N> ones()
{
    vec<T, N> v;
    std::fill(v.m_data, v.m_data + N, static_cast<T>(1));
    return v;
};

template <typename T, size_t N>
vec<T, N> zeros()
{
    vec<T, N> v;
    std::fill(v.m_data, v.m_data + N, static_cast<T>(0));
    return v;
};

} // namespace radiant