#pragma once

#include <core/vec.hpp>

namespace radiant
{
struct Ray
{
    vec3f m_origin;
    vec3f m_direction;

    Ray() = default;
    Ray(const vec3f& origin, const vec3f& direction) : m_origin(origin), m_direction(direction) {}

    vec3f at(f32 t) const { return m_origin + t * m_direction; }
};

} // namespace radiant
