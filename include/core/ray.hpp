#pragma once

#include <core/vec.hpp>

namespace radiant
{
struct Ray
{
    vec3 m_origin;
    vec3 m_direction;

    Ray() = default;
    Ray(const vec3& origin, const vec3& direction) : m_origin(origin), m_direction(direction) {}

    vec3 at(Scalar t) const { return m_origin + t * m_direction; }
};

} // namespace radiant
