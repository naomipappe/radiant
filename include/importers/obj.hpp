#pragma once

#include <memory>
#include <utility>
#include <filesystem>

#include "core/shapes/triangle.hpp"

namespace radiant
{
Mesh import_mesh(const std::filesystem::path& path);
}