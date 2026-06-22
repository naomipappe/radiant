#include "core/shape.hpp"
#include "core/math.hpp"
#include "core/resource_manager.h"

namespace radiant
{
Shape make_sphere(const vec3& position, const float radius)
{
    Shape shape;
    shape.m_type        = ShapeType::Sphere;
    shape.sphere.radius = radius;
    shape.m_centroid    = position;

    for (u32 i = 0; i < 3; ++i)
    {
        shape.aabb_min[i] = position[i] - radius;
        shape.aabb_max[i] = position[i] + radius;
    }

    return shape;
}

std::optional<Intersection> intersect_sphere(const Shape& s, const Ray& r, Scalar tmin, Scalar tmax)
{
    const vec3   center_to_ray_origin = s.m_centroid - r.m_origin;
    const Scalar a                    = r.m_direction.length_squared();
    const Scalar h                    = dot(r.m_direction, center_to_ray_origin);
    const Scalar c                    = center_to_ray_origin.length_squared() - s.sphere.radius * s.sphere.radius;
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
    vec3 outward_normal = (p - s.m_centroid) / s.sphere.radius;

    return std::make_optional<Intersection>(p, outward_normal, root);
}

Shape make_triangle(const u64 mesh_index, const u64 triangle_index)
{
    Shape       s;
    const u32*  indices       = &ResourceManager::get_mesh(mesh_index).m_indices[3 * triangle_index];
    const vec3& p1            = ResourceManager::get_mesh(mesh_index).m_positions[indices[0]];
    const vec3& p2            = ResourceManager::get_mesh(mesh_index).m_positions[indices[1]];
    const vec3& p3            = ResourceManager::get_mesh(mesh_index).m_positions[indices[2]];
    s.m_centroid              = 0.3333 * (p1 + p2 + p3);
    s.m_type                  = ShapeType::Triangle;
    s.triangle.mesh_index     = mesh_index;
    s.triangle.triangle_index = triangle_index;

    s.aabb_min = pointwise_min(s.aabb_min, p1);
    s.aabb_min = pointwise_min(s.aabb_min, p2);
    s.aabb_min = pointwise_min(s.aabb_min, p3);

    s.aabb_max = pointwise_max(s.aabb_max, p1);
    s.aabb_max = pointwise_max(s.aabb_max, p2);
    s.aabb_max = pointwise_max(s.aabb_max, p3);
    return s;
}

std::optional<Intersection> intersect_triangle(const Shape& s, const Ray& r, const Scalar tmin, const Scalar tmax)
{
    assert(tmin > 0);
    const u32*  indices = &ResourceManager::get_mesh(s.triangle.mesh_index).m_indices[3 * s.triangle.triangle_index];
    const vec3& p1      = ResourceManager::get_mesh(s.triangle.mesh_index).m_positions[indices[0]];
    const vec3& p2      = ResourceManager::get_mesh(s.triangle.mesh_index).m_positions[indices[1]];
    const vec3& p3      = ResourceManager::get_mesh(s.triangle.mesh_index).m_positions[indices[2]];

    const vec3 e1 = p2 - p1;
    const vec3 e2 = p3 - p1;

    const Scalar denom = det(-r.m_direction, e1, e2);
    if (std::abs(denom) < eps)
    {
        return std::nullopt; // Ray parallel to triangle
    }

    const vec3 rhs = r.m_origin - p1;
    Scalar     t   = det(rhs, e1, e2) / denom;

    if (t < tmin || t > tmax)
    {
        return std::nullopt; // Intersection outside ray bounds
    }

    const Scalar u = det(-r.m_direction, rhs, e2) / denom;
    const Scalar v = det(-r.m_direction, e1, rhs) / denom;

    if (u < 0 || v < 0 || u + v > 1)
    {
        return std::nullopt; // Intersection outside triangle
    }

    const vec3& n1 = ResourceManager::get_mesh(s.triangle.mesh_index).m_normals[indices[0]];
    const vec3& n2 = ResourceManager::get_mesh(s.triangle.mesh_index).m_normals[indices[1]];
    const vec3& n3 = ResourceManager::get_mesh(s.triangle.mesh_index).m_normals[indices[2]];

    vec3 n_hit = ((1 - u - v) * n1 + u * n2 + v * n3).normalize();
    if (r.m_direction.dot(n_hit) > 0)
    {
        n_hit = -n_hit;
    }
    return std::make_optional<Intersection>(r.m_origin + t * r.m_direction, n_hit, t);
}
Shape                       make_quad(const vec3& position, const vec3& normal) {}
std::optional<Intersection> intersect_quad(const Ray& r, Scalar tmin, Scalar tmax) {}
} // namespace radiant
