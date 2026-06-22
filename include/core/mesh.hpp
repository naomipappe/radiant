#pragma once
#include <core/vec.hpp>

namespace radiant
{
struct Mesh
{
    u32         m_num_triangles{ 0 }, m_num_positions{ 0 };
    const u32*  m_indices   = nullptr;
    const vec3* m_positions = nullptr;
    const vec3* m_normals   = nullptr;
};
} // namespace radiant