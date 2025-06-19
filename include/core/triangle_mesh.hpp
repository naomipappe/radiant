#pragma once

#include "core/primitive.hpp"
#include "core/types.hpp"


namespace radiant
{

struct StaticTriangleMesh : public Primitive
{
    StaticTriangleMesh() = default;
    u32         m_num_triangles{ 0 }, m_num_vertices{ 0 };
    const u32*  m_indices   = nullptr;
    const vec3* m_positions = nullptr;
    const vec3* m_normals   = nullptr;
    const vec3* m_tangents  = nullptr;
    const vec2* m_uv        = nullptr;
};
} // namespace radiant