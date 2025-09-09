#pragma once

#include <memory>
#include <utility>
#include <filesystem>

#include "core/shapes/triangle.hpp"
#include "core/triangle_mesh.hpp"

namespace radiant
{
std::pair<std::shared_ptr<StaticTriangleMesh>, std::vector<std::shared_ptr<Triangle>>>
import_mesh(const std::filesystem::path& path);
}