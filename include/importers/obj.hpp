#pragma once

#include <core/mesh.hpp>
#include <filesystem>


namespace radiant
{
Mesh import_mesh(const std::filesystem::path& path);
}