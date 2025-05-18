#pragma once

#include "core/vec.hpp"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <limits>
#include <random>

namespace radiant
{

template <typename T>
    requires std::is_floating_point_v<T>
T random()
{
    static std::uniform_real_distribution<T> distribution(0.0, 1.0);
    static std::mt19937                      generator;
    return distribution(generator);
}

template <typename T>
    requires std::is_floating_point_v<T>
T random(T min, T max)
{
    return min + (max - min) * random<T>();
}

template <typename T, size_t N>
vec<T, N> random()
{
    vec<T, N> v{};
    for (u32 i = 0; i < N; ++i)
    {
        v[i] = random<T>();
    }
    return v;
}

template <typename T, size_t N>
vec<T, N> random(T min, T max)
{
    vec<T, N> v;
    for (u32 i = 0; i < N; ++i)
    {
        v[i] = random<T>(min, max);
    }
    return v;
}

// Sample a random unit vector on the hemisphere, normalizing
inline vec3 sample_sphere_hemisphere_rejection(const vec3& normal)
{
    // First, generate a vector that falls into a [-1,-1]X[-1,1]x[-1,1] cube
    // then, reject if its length is > 1
    vec3 random_unit_vector;
    while (true)
    {
        vec3   p              = random<Scalar, 3>(-1.0, 1.0);
        Scalar length_squared = p.length_squared();
        if (std::numeric_limits<Scalar>::epsilon() <= length_squared && length_squared <= 1.0)
        {
            random_unit_vector = normalized(p);
            break;
        }
    }
    if (dot(random_unit_vector, normal) > std::numeric_limits<Scalar>::epsilon())
    {
        return random_unit_vector;
    }
    else
    {
        return -random_unit_vector;
    }
}

inline vec3 sample_unit_disk()
{
    vec3 candidate;
    do
    {
        candidate = vec3(random(-1.0, 1.0), random(-1.0, 1.0), 0.0);
    } while (candidate.length_squared() >= 1.0);
    return candidate;
}

} // namespace radiant