#include <cmath>
#include <core/sphere.hpp>
#include <cassert>

namespace radiant
{
Sphere::Sphere(const vec3& center, Scalar radius) : m_center(center), m_radius(std::fmax(0, radius)), m_material(nullptr) {}

Sphere::Sphere(const vec3& center, Scalar radius, Material* material) :
    m_center(center), m_radius(std::fmax(0, radius)), m_material(material) {};

bool Sphere::test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const
{
    std::optional<Intersection> intersection = intersect(r, tmin, tmax);
    return intersection.has_value();
}

std::optional<Intersection> Sphere::intersect(const Ray& r, Scalar tmin, Scalar tmax) const
{
    const vec3 center_to_ray_origin = m_center - r.m_origin;
    const Scalar   a                    = r.m_direction.length_squared();
    const Scalar   h                    = dot(r.m_direction, center_to_ray_origin);
    const Scalar   c                    = center_to_ray_origin.length_squared() - m_radius * m_radius;
    const Scalar   D                    = h * h - a * c;

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

    return std::make_optional<Intersection>(p, outward_normal, root, m_material);
}
} // namespace radiant
