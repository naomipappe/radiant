#pragma once

#include <core/vec.hpp>

namespace radiant
{
class Material;
// The decision here is that we always use the dot product to determine whether the ray comes from inside or not
struct SurfaceIntersection
{
    SurfaceIntersection() = default;
    SurfaceIntersection(const vec3& intersection, const vec3& normal, Scalar t) :
        m_intersection(intersection), m_normal(normal), m_t(t)
    {}
    vec3      m_intersection;
    vec3      m_normal;
    Scalar    m_t;
    Material* m_material;
};
} // namespace radiant