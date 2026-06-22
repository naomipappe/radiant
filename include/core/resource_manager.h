#pragma once

#include "core/mesh.hpp"
#include "core/material.hpp"

#include <vector>

namespace radiant
{
// For now, this manages only meshes as no textures are used.
// This is done to avoid implicit global meshes storage somewhere in the code and make it an explicit ugly
class ResourceManager
{
  public:
    ResourceManager(const ResourceManager&)            = delete;
    ResourceManager(ResourceManager&&)                 = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&)      = delete;

    static ResourceManager& get()
    {
        static ResourceManager instance;
        return instance;
    }

    static u64 add_mesh(Mesh&& mesh)
    {
        get().meshes.push_back(std::move(mesh));
        return get().meshes.size() - 1;
    }
    static Mesh& get_mesh(const u64 idx) { return get().meshes[idx]; }

  private:
    ResourceManager()  = default;
    ~ResourceManager() = default;

  private:
    // TODO: This assumes that meshes are basically loaded once
    // If any mesh is deleted - all triangle indices are invalidated
    // For now, this is good enough, as raytracer is static
    std::vector<Mesh> meshes{};
};
} // namespace radiant