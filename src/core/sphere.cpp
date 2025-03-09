#include <core/sphere.hpp>
#include <cassert>

namespace radiant
{
Sphere::Sphere(const vec3f& center, f32 radius) : m_center(center), m_radius(std::fmax(0, radius)) {}

bool Sphere::test_intersection(const Ray& r, f32 tmin, f32 tmax) const
{
    std::optional<Intersection> intersection = intersect(r, tmin, tmax);
    return intersection.has_value();
}

std::optional<Intersection> Sphere::intersect(const Ray& r, f32 tmin, f32 tmax) const
{
    const vec3f center_to_ray_origin = m_center - r.m_origin;
    const f32   a                    = r.m_direction.length_squared();
    const f32   h                    = dot(r.m_direction, center_to_ray_origin);
    const f32   c                    = center_to_ray_origin.length_squared() - m_radius * m_radius;
    const f32   D                    = h * h - a * c;

    f32 root = -1.0f;
    if (D < 0)
    {
        return std::nullopt;
    }

    const f32 sqrtd  = std::sqrt(D);
    const f32 root_l = (h - sqrtd) / a;
    const f32 root_r = (h + sqrtd) / a;

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
    vec3f p              = r.at(root);
    vec3f outward_normal = (p - m_center) / m_radius;

    return std::make_optional<Intersection>(p, outward_normal, root);
}
} // namespace radiant
