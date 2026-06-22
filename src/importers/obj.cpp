#include "rapidobj/rapidobj.hpp"

#include "importers/obj.hpp"

#include "core/cache/buffer_storage.hpp"
#include "core/types.hpp"
#include "fmt/base.h"
#include "fmt/printf.h"

#include <cstdlib>
#include <span>

namespace radiant
{
Mesh import_mesh(const std::filesystem::path& path)
{
    rapidobj::Result result = rapidobj::ParseFile(path);

    if (result.error)
    {
        fmt::fprintf(stderr, "{}", result.error.line);
        std::abort();
    }

    if (!rapidobj::Triangulate(result))
    {
        std::abort();
    }

    Mesh mesh{};

    const rapidobj::Attributes& attrib = result.attributes;
    const rapidobj::Shapes&     shapes = result.shapes;

    std::vector<vec3>     positions;
    std::vector<vec3>     normals;
    std::vector<uint32_t> indices;

    u64 reserve_positions = 0;
    u64 reserve_normals   = 0;
    for (const rapidobj::Shape& shape : shapes)
    {
        for (const rapidobj::Index& idx : shape.mesh.indices)
        {
            reserve_positions += idx.position_index >= 0;
            reserve_normals += idx.normal_index >= 0;
        }
    }

    positions.reserve(reserve_positions);
    normals.reserve(reserve_normals);

    for (const rapidobj::Shape& shape : shapes)
    {
        for (const rapidobj::Index& idx : shape.mesh.indices)
        {
            if (idx.position_index >= 0)
            {
                const i32 offset = 3 * idx.position_index;
                positions.emplace_back(
                    attrib.positions[offset + 0], attrib.positions[offset + 1], attrib.positions[offset + 2]);
            }
            if (idx.normal_index >= 0)
            {
                const i32 offset = 3 * idx.normal_index;
                normals.emplace_back(attrib.normals[offset + 0], attrib.normals[offset + 1], attrib.normals[offset + 2]);
            }

            indices.push_back(static_cast<u32>(indices.size()));
        }
    }

    mesh.m_positions = BufferStorage<vec3>::instance().store(positions);
    mesh.m_normals   = BufferStorage<vec3>::instance().store(normals);
    mesh.m_indices   = BufferStorage<u32>::instance().store(indices);

    mesh.m_num_triangles = indices.size() / 3;
    mesh.m_num_positions = positions.size();

    return mesh;
}
} // namespace radiant