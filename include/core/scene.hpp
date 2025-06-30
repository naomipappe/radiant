#pragma once

#include "core/material.hpp"
#include "core/primitive.hpp"
#include "core/triangle_mesh.hpp"

#include <memory>
#include <vector>

namespace radiant
{

struct Scene
{
    std::vector<std::shared_ptr<StaticTriangleMesh>> m_meshes;
    std::vector<std::shared_ptr<Material>>           m_materials;
    std::vector<std::shared_ptr<GeometricPrimitive>> m_primitives;
};

} // namespace radiant