#pragma once

#include "core/shapes/triangle.hpp"
#include "core/triangle_mesh.hpp"
#include <memory>
#include <utility>

namespace radiant
{
std::pair<std::shared_ptr<StaticTriangleMesh>, std::vector<std::shared_ptr<Triangle>>> import_mesh(std::string path);
}