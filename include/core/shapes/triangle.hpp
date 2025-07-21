#pragma once

#include "core/math.hpp"
#include "core/ray.hpp"
#include "core/shapes/shape.hpp"
#include "core/surface_interaction.hpp"
#include "core/triangle_mesh.hpp"
#include "core/types.hpp"
#include "core/vec.hpp"

#include <cassert>
#include <limits>
#include <memory>
#include <optional>
#include <vector>

namespace radiant
{
struct Triangle final : public Shape
{
    Triangle(u32 mesh_idx, u32 triangle_idx) : m_mesh_index(mesh_idx), m_triangle_index(triangle_idx)
    {
        const u32*  indices = &(*g_meshes)[m_mesh_index]->m_indices[3 * m_triangle_index];
        const vec3& p1      = (*g_meshes)[m_mesh_index]->m_positions[indices[0]];
        const vec3& p2      = (*g_meshes)[m_mesh_index]->m_positions[indices[1]];
        const vec3& p3      = (*g_meshes)[m_mesh_index]->m_positions[indices[2]];
        m_centroid          = 0.3333 * (p1 + p2 + p3);

        aabb_min = pointwise_min(aabb_min, p1);
        aabb_min = pointwise_min(aabb_min, p2);
        aabb_min = pointwise_min(aabb_min, p3);

        aabb_max = pointwise_max(aabb_max, p1);
        aabb_max = pointwise_max(aabb_max, p2);
        aabb_max = pointwise_max(aabb_max, p3);
    }
    ~Triangle() override = default;

    static std::vector<std::shared_ptr<StaticTriangleMesh>>* g_meshes;

    u32 m_mesh_index{ 0 };
    u32 m_triangle_index{ 0 };

    bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        return intersect(r, tmin, tmax).has_value();
    }

    std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        assert(tmin > 0);
        const std::vector<std::shared_ptr<StaticTriangleMesh>>& meshes = *g_meshes;
        const u32*  indices = &meshes[m_mesh_index]->m_indices[3 * m_triangle_index];
        const vec3& p1      = meshes[m_mesh_index]->m_positions[indices[0]];
        const vec3& p2      = meshes[m_mesh_index]->m_positions[indices[1]];
        const vec3& p3      = meshes[m_mesh_index]->m_positions[indices[2]];

        vec3 e1 = p2 - p1;
        vec3 e2 = p3 - p1;

        Scalar denom = det(-r.m_direction, e1, e2);
        if (std::abs(denom) < eps)
        {
            return std::nullopt; // Ray parallel to triangle
        }

        vec3   rhs = r.m_origin - p1;
        Scalar t   = det(rhs, e1, e2) / denom;

        if (t < tmin || t > tmax)
        {
            return std::nullopt; // Intersection outside ray bounds
        }

        Scalar u = det(-r.m_direction, rhs, e2) / denom;
        Scalar v = det(-r.m_direction, e1, rhs) / denom;

        if (u < 0 || v < 0 || u + v > 1)
        {
            return std::nullopt; // Intersection outside triangle
        }

        const vec3& n1 = meshes[m_mesh_index]->m_normals[indices[0]];
        const vec3& n2 = meshes[m_mesh_index]->m_normals[indices[1]];
        const vec3& n3 = meshes[m_mesh_index]->m_normals[indices[2]];

        vec3 n_hit = ((1 - u - v) * n1 + u * n2 + v * n3).normalize();
        if (r.m_direction.dot(n_hit) > 0)
        {
            n_hit = -n_hit;
        }
        return std::make_optional<SurfaceIntersection>(r.m_origin + t * r.m_direction, n_hit, t);
    }
};
} // namespace radiant