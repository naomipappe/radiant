#pragma once

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

} // namespace radiant