#pragma once

#include "core/ray.hpp"
#include "core/shapes/shape.hpp"
#include "core/intersection.hpp"
#include "core/types.hpp"
#include "core/vec.hpp"

#include <optional>
#include <vector>

namespace radiant
{

struct Mesh
{
    u32         m_num_triangles{ 0 }, m_num_positions{ 0 };
    const u32*  m_indices   = nullptr;
    const vec3* m_positions = nullptr;
    const vec3* m_normals   = nullptr;
};

inline std::vector<Mesh> meshes;

struct Triangle : Shape
{
    u64 m_mesh_index{ 0 };
    u64 m_triangle_index{ 0 };

    Triangle(u32 mesh_idx, u32 triangle_idx);
    ~Triangle() override = default;

    [[nodiscard]] bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override;
    [[nodiscard]] std::optional<Intersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override;
};
} // namespace radiant