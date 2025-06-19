#pragma once

#include "core/math.hpp"
#include "core/ray.hpp"
#include "core/shapes/shape.hpp"
#include "core/surface_interaction.hpp"
#include "core/triangle_mesh.hpp"
#include "core/types.hpp"
#include "core/vec.hpp"
#include <limits>
#include <memory>
#include <optional>
#include <vector>
#include <array>

namespace radiant
{
struct Triangle : public Shape
{
    static std::vector<std::shared_ptr<StaticTriangleMesh>>* g_meshes;

    u32 m_mesh_index{ 0 };
    u32 m_triangle_index{ 0 };

    bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        return intersect(r, tmin, tmax).has_value();
    }

    std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        const std::vector<std::shared_ptr<StaticTriangleMesh>>& meshes = *g_meshes;

        const u32*  indices = &meshes[m_mesh_index]->m_indices[3 * m_triangle_index];
        const vec3& p1      = meshes[m_mesh_index]->m_positions[indices[0]];
        const vec3& p2      = meshes[m_mesh_index]->m_positions[indices[1]];
        const vec3& p3      = meshes[m_mesh_index]->m_positions[indices[2]];

        // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

        // 1. Check if the ray is parallel to the plane on which the triangle lies
        vec3   e1    = p2 - p1;
        vec3   e2    = p3 - p1;
        vec3   p_n   = cross(e1, e2).normalize();
        Scalar p_dot = dot(r.m_direction, p_n);
        if (-eps / 2 <= p_dot && p_dot <= eps / 2)
        {
            return std::nullopt;
        }
        // 2. Check if the intersection point with the plane is inside the

        // Calculate the determinant of the system in closed form
        const auto& [d1, d2, d3]    = r.m_direction.m_data;
        const auto& [e11, e12, e13] = e1.m_data;
        const auto& [e21, e22, e23] = e2.m_data;

        Scalar denom = det(-r.m_direction, e1, e2);
        if (-eps / 2 <= denom && denom <= eps / 2)
        {
            return std::nullopt;
        }

        vec3   rhs = r.m_origin - p1;
        Scalar t   = det(rhs, e1, e2);
        if (t >= eps / 2)
        {
            Scalar u = det(-r.m_direction, rhs, e2) / denom;
            Scalar v = det(-r.m_direction, e1, rhs) / denom;

            const vec3& n1 = meshes[m_mesh_index]->m_normals[indices[0]];
            const vec3& n2 = meshes[m_mesh_index]->m_normals[indices[1]];
            const vec3& n3 = meshes[m_mesh_index]->m_normals[indices[2]];

            return std::make_optional<SurfaceIntersection>(
                r.m_origin + t * r.m_direction, ((1 - u - v) * n1 + u * n2 + v * n3).normalize(), t);
        }
        return std::nullopt;
    }
};
} // namespace radiant