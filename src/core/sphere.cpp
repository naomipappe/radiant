#include <cmath>
#include <cassert>

#include <core/vec.hpp>
#include <core/shapes/sphere.hpp>

namespace radiant
{
Sphere::Sphere(const vec3& center, Scalar radius) : m_center(center), m_radius(std::fmax(0, radius)) {}

bool Sphere::test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const
{
    std::optional<SurfaceIntersection> intersection = intersect(r, tmin, tmax);
    return intersection.has_value();
}

std::optional<SurfaceIntersection> Sphere::intersect(const Ray& r, Scalar tmin, Scalar tmax) const
{
    const vec3   center_to_ray_origin = m_center - r.m_origin;
    const Scalar a                    = r.m_direction.length_squared();
    const Scalar h                    = dot(r.m_direction, center_to_ray_origin);
    const Scalar c                    = center_to_ray_origin.length_squared() - m_radius * m_radius;
    const Scalar D                    = h * h - a * c;

    Scalar root = -1.0f;
    if (D < 0)
    {
        return std::nullopt;
    }

    const Scalar sqrtd  = std::sqrt(D);
    const Scalar root_l = (h - sqrtd) / a;
    const Scalar root_r = (h + sqrtd) / a;

    if (root_l > tmin && root_l < tmax)
    {
        root = root_l;
    }
    else if (root_r > tmin && root_r < tmax)
    {
        root = root_r;
    }
    else
    {
        return std::nullopt;
    }

    assert(root >= 0.0f);
    vec3 p              = r.at(root);
    vec3 outward_normal = (p - m_center) / m_radius;

    return std::make_optional<SurfaceIntersection>(p, outward_normal, root);
}
} // namespace radiant
