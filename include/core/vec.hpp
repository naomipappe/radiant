#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <cmath>
#include <core/types.hpp>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <immintrin.h>
#include <xmmintrin.h>

namespace radiant
{

template <typename T, size_t N>
struct vec;

using vec2 = vec<Scalar, 2>;
using vec3 = vec<Scalar, 3>;
using vec4 = vec<Scalar, 4>;

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
    T data[N];

    vec() : data{} {}
    explicit vec(const T* data) { std::copy(data, data + N, this->data); }
    explicit vec(T* data) { std::copy(data, data + N, this->data); }
    explicit vec(const T value) { std::fill(data, data + N, value); }

    template <typename... Args>
    constexpr explicit vec(Args&&... args) : data{ static_cast<T>(std::forward<Args>(args))... }
    {
        static_assert(sizeof...(Args) == N, "Number of arguments must match vector size");
    }

    const T& operator[](u32 i) const { return data[i]; };
    T&       operator[](u32 i) { return data[i]; };

    bool is_zero() const
    {
        bool result = true;
        for (u32 i = 0; i < N; ++i)
        {
            result = result && (std::fabs(data[i]) < std::numeric_limits<Scalar>::epsilon());
        }
        return result;
    }

    vec operator-() const
    {
        vec result;
        for (u32 i = 0; i < N; ++i)
        {
            result.data[i] = -data[i]; // Proper negation
        }
        return result;
    }

    vec& operator+=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            data[i] += v.data[i];
        }
        return *this;
    }

    vec& operator-=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            data[i] -= v.data[i];
        }
        return *this;
    }

    vec& operator*=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            data[i] *= v.data[i];
        }
        return *this;
    }

    vec& operator/=(const vec& v)
    {
        for (u32 i = 0; i < N; ++i)
        {
            data[i] /= v.data[i];
        }
        return *this;
    }

    vec& operator*=(T scalar)
    {
        for (u32 i = 0; i < N; ++i)
        {
            data[i] *= scalar;
        }
        return *this;
    }

    vec& operator/=(T scalar)
    {
        for (u32 i = 0; i < N; ++i)
        {
            data[i] /= scalar;
        }
        return *this;
    }

    vec& operator+=(T scalar)
    {
        for (u32 i = 0; i < N; ++i)
        {
            data[i] += scalar;
        }
        return *this;
    }

    T dot(const vec& v) const
    {
        T result{ 0 };
        for (u32 i = 0; i < N; ++i)
        {
            result += data[i] * v.data[i];
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

template <>
struct vec<f32, 4>
{
  private:
    __m128 val;
    void   store(__m128 v) { val = v; }
    vec(__m128 v) { store(v); }

  public:
    vec() { val = _mm_setzero_ps(); }
    explicit vec(float w) { val = _mm_set1_ps(w); }
    vec(float x, float y, float z, float w) { val = _mm_set_ps(w, z, y, x); }
    explicit vec(const f32* data) { val = _mm_set_ps(data[3], data[2], data[1], data[0]); }
    explicit vec(f32* data) { val = _mm_set_ps(data[3], data[2], data[1], data[0]); }

    vec(const vec<f32, 4>& other) : val(other.val) {}

    f32 x() const { return std::bit_cast<float>(_mm_extract_ps(val, 0)); }
    f32 y() const { return std::bit_cast<float>(_mm_extract_ps(val, 1)); }
    f32 z() const { return std::bit_cast<float>(_mm_extract_ps(val, 2)); }
    f32 w() const { return std::bit_cast<float>(_mm_extract_ps(val, 3)); }

    const f32 operator[](u32 i) const
    {
        assert(i < 4);
        switch (i)
        {
            case 0:
            {
                return std::bit_cast<f32>(_mm_extract_ps(val, 0));
            }
            case 1:
            {
                return std::bit_cast<f32>(_mm_extract_ps(val, 1));
            }
            case 2:
            {
                return std::bit_cast<f32>(_mm_extract_ps(val, 2));
            }
            case 3:
            {
                return std::bit_cast<f32>(_mm_extract_ps(val, 3));
            }
        }
    }

    bool is_zero() const
    {
        __m128 sign_bit = _mm_set1_ps(-0.0f);
        __m128 absv     = _mm_andnot_ps(sign_bit, val);
        __m128 result   = _mm_cmple_ps(absv, _mm_set1_ps(std::numeric_limits<f32>::epsilon()));
        return _mm_movemask_ps(result) == 0xF;
    }

    vec operator-() const
    {
        __m128 sign_bit = _mm_set1_ps(-0.0f);
        return _mm_xor_ps(sign_bit, val);
    }

    vec& operator+=(const vec& v)
    {
        val = _mm_add_ps(val, v.val);
        return *this;
    }

    vec& operator-=(const vec& v)
    {
        val = _mm_sub_ps(val, v.val);
        return *this;
    }

    vec& operator*=(const vec& v)
    {
        val = _mm_mul_ps(val, v.val);
        return *this;
    }

    vec& operator/=(const vec& v)
    {
        val = _mm_div_ps(val, v.val);
        return *this;
    }

    vec& operator*=(f32 scalar)
    {
        val = _mm_mul_ps(val, _mm_set1_ps(scalar));
        return *this;
    }

    vec& operator/=(f32 scalar)
    {
        val = _mm_div_ps(val, _mm_set1_ps(scalar));
        return *this;
    }

    vec& operator+=(f32 scalar)
    {
        val = _mm_add_ps(val, _mm_set1_ps(scalar));
        return *this;
    }

    vec& operator-=(f32 scalar)
    {
        val = _mm_sub_ps(val, _mm_set1_ps(scalar));
        return *this;
    }

    f32 dot(const vec& v) const
    {
        // 0b1111 means that all 4 lanes of the _m128 are multipled
        // 0b0001 means that the result is storen in lane 0
        return _mm_cvtss_f32(_mm_dp_ps(val, v.val, 0b11110001));
    }

    vec& normalize()
    {
        *this /= this->length();
        return *this;
    }

    Scalar length() const { return std::sqrt(dot(*this)); }
    Scalar length_squared() const { return dot(*this); }
};

template <>
struct vec<f64, 4>
{
  private:
    __m256d val;
    void    store(__m256d v) { val = v; }
    vec(__m256d v) { store(v); }

  public:
    vec() { val = _mm256_setzero_pd(); }
    explicit vec(f64 w) { val = _mm256_set1_pd(w); }
    vec(f64 x, f64 y, f64 z, f64 w) { val = _mm256_set_pd(w, z, y, x); }
    explicit vec(const f64* data) { val = _mm256_set_pd(data[3], data[2], data[1], data[0]); }
    explicit vec(f64* data) { val = _mm256_set_pd(data[3], data[2], data[1], data[0]); }
    vec(const vec<f64, 4>& other) : val(other.val) {}

    f64 x() const { return std::bit_cast<f64>(_mm256_extract_epi64(val, 0)); }
    f64 y() const { return std::bit_cast<f64>(_mm256_extract_epi64(val, 1)); }
    f64 z() const { return std::bit_cast<f64>(_mm256_extract_epi64(val, 2)); }
    f64 w() const { return std::bit_cast<f64>(_mm256_extract_epi64(val, 3)); }

    const f64 operator[](u32 i) const
    {
        assert(i < 4);
        switch (i)
        {
            case 0:
            {
                return std::bit_cast<f64>(_mm256_extract_epi64(val, 0));
            }
            case 1:
            {
                return std::bit_cast<f64>(_mm256_extract_epi64(val, 1));
            }
            case 2:
            {
                return std::bit_cast<f64>(_mm256_extract_epi64(val, 2));
            }
            case 3:
            {
                return std::bit_cast<f64>(_mm256_extract_epi64(val, 3));
            }
        }
    }

    bool is_zero() const
    {
        __m256d sign_bit = _mm256_set1_pd(-0.0f);
        __m256d absv     = _mm256_andnot_pd(sign_bit, val);
        __m256d result   = _mm256_cmp_pd(absv, _mm256_set1_pd(std::numeric_limits<f64>::epsilon()), _CMP_LE_OS);
        return _mm256_movemask_pd(result) == 0xF;
    }

    vec operator-() const
    {
        __m256d sign_bit = _mm256_set1_pd(-0.0f);
        return _mm256_xor_pd(sign_bit, val);
    }

    vec& operator+=(const vec& v)
    {
        val = _mm256_add_pd(val, v.val);
        return *this;
    }

    friend vec operator+(const vec& a, const vec& b) { return _mm256_add_pd(a.val, b.val); }

    template <typename T> requires ScalarT<T>
    friend vec operator+(const vec& a, T b) { return _mm256_add_pd(a.val, _mm256_set1_pd(b)); }

    vec& operator-=(const vec& v)
    {
        val = _mm256_sub_pd(val, v.val);
        return *this;
    }

    friend vec operator-(const vec& a, const vec& b) { return _mm256_sub_pd(a.val, b.val); }

    template <typename T>
        requires ScalarT<T>
    friend vec operator-(const vec& a, T b)
    {
        return _mm256_sub_pd(a.val, _mm256_set1_pd(b));
    }

    vec& operator*=(const vec& v)
    {
        val = _mm256_mul_pd(val, v.val);
        return *this;
    }

    friend vec operator*(const vec& a, const vec& b) { return _mm256_mul_pd(a.val, b.val); }

    template <typename T>
        requires ScalarT<T>
    friend vec operator*(const vec& a, T b)
    {
        return _mm256_mul_pd(a.val, _mm256_set1_pd(b));
    }

    template <typename T>
        requires ScalarT<T>
    friend vec operator*(T b, const vec& a)
    {
        return _mm256_mul_pd(a.val, _mm256_set1_pd(b));
    }

    vec& operator/=(const vec& v)
    {
        val = _mm256_div_pd(val, v.val);
        return *this;
    }

    friend vec operator/(const vec& a, const vec& b) { return _mm256_div_pd(a.val, b.val); }

    vec& operator*=(f64 scalar)
    {
        val = _mm256_mul_pd(val, _mm256_set1_pd(scalar));
        return *this;
    }

    vec& operator/=(f64 scalar)
    {
        val = _mm256_div_pd(val, _mm256_set1_pd(scalar));
        return *this;
    }

    vec& operator+=(f64 scalar)
    {
        val = _mm256_add_pd(val, _mm256_set1_pd(scalar));
        return *this;
    }

    vec& operator-=(f64 scalar)
    {
        val = _mm256_sub_pd(val, _mm256_set1_pd(scalar));
        return *this;
    }

    f64 dot(const vec& v) const
    {
        __m256d mul  = _mm256_mul_pd(val, v.val);
        __m256d hadd = _mm256_hadd_pd(mul, mul);
        __m128d high = _mm256_extractf128_pd(hadd, 1);
        __m128d low  = _mm256_castpd256_pd128(hadd);
        return _mm_cvtsd_f64(_mm_add_pd(low, high));
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
    vec<T, N> v(static_cast<T>(1));
    return v;
};

template <typename T, size_t N>
vec<T, N> zeros()
{
    vec<T, N> v(static_cast<T>(0));
    return v;
};

} // namespace radiant
