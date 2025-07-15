#include "rapidobj/rapidobj.hpp"

#include "importers/obj.hpp"

#include "core/cache/buffer_storage.hpp"
#include "core/shapes/triangle.hpp"
#include "core/triangle_mesh.hpp"
#include "core/types.hpp"
#include "fmt/base.h"
#include "fmt/printf.h"

#include <cstdlib>
#include <memory>
#include <span>

namespace radiant
{
std::pair<std::shared_ptr<StaticTriangleMesh>, std::vector<std::shared_ptr<Triangle>>> import_mesh(std::string path)
{
    rapidobj::Result result = rapidobj::ParseFile(path, rapidobj::MaterialLibrary::Ignore());

    if (result.error)
    {
        fmt::fprintf(stderr, "{}", result.error.line);
        std::abort();
    }

    bool success = rapidobj::Triangulate(result);

    if (!success)
    {
        std::abort();
    }

    std::shared_ptr<StaticTriangleMesh> mesh = std::make_shared<StaticTriangleMesh>();

    const auto& attrib = result.attributes;
    const auto& shapes = result.shapes;

    std::vector<vec3>     positions;
    std::vector<vec3>     normals;
    std::vector<uint32_t> indices;

    for (const auto& shape : shapes)
    {
        for (const auto& idx : shape.mesh.indices)
        {
            vec3 pos(0.0), norm(0.0);

            // Position
            if (idx.position_index >= 0)
            {
                int offset = 3 * idx.position_index;
                pos        = vec3(attrib.positions[offset + 0], attrib.positions[offset + 1], attrib.positions[offset + 2]);
            }

            // Normal
            if (idx.normal_index >= 0)
            {
                int offset = 3 * idx.normal_index;
                norm       = vec3(attrib.normals[offset + 0], attrib.normals[offset + 1], attrib.normals[offset + 2]);
            }

            positions.push_back(pos);
            normals.push_back(norm);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }

    mesh->m_positions = BufferStorage<vec3>::instance().store(positions);
    mesh->m_normals   = BufferStorage<vec3>::instance().store(normals);
    mesh->m_indices   = BufferStorage<u32>::instance().store(indices);

    mesh->m_num_triangles = indices.size() / 3;
    mesh->m_num_positions = positions.size();

    u32 mesh_idx = Triangle::g_meshes->size();
    Triangle::g_meshes->push_back(mesh);
    std::vector<std::shared_ptr<Triangle>> triangles;
    triangles.reserve(mesh->m_num_triangles);
    for (u32 t = 0; t < mesh->m_num_triangles; ++t)
    {
        triangles.emplace_back(std::make_shared<Triangle>(mesh_idx, t));
    }

    return { mesh, triangles };
}
} // namespace radiant