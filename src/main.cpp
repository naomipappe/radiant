#include "core/acceleration_structures/bvh_aggregate.hpp"
#include "core/camera.hpp"
#include "core/color.hpp"
#include "core/material.hpp"
#include "core/primitive.hpp"
#include "core/render_target.hpp"
#include "fmt/base.h"
#include "importers/obj.hpp"
#include <garbage/garbage_dump.hpp>

#include <core/constants.hpp>
#include <core/ray.hpp>
#include <core/vec.hpp>
#include <core/acceleration_structures/linear_aggregate.hpp>
#include <core/shapes/sphere.hpp>
#include <core/shapes/triangle.hpp>

#include <filesystem>
#include <memory>
#include <vector> // TODO: Remove STL when bored

using namespace radiant;

int main(int argc, char* argv[])
{
    fmt::println("Starting Radiant");
    fmt::println("Argument count {}", argc);
    fmt::println("Arguments {}", *argv);

    std::filesystem::path destination;

    std::filesystem::path asset_source;
    // TODO: proper argument parsing and cmd-line interface (python script?)
    if (argc >= 2)
    {
        destination = std::filesystem::path(argv[1]);
    }
    else
    {
        destination = std::filesystem::current_path().parent_path() / "render.png";
    }
    if (argc >= 3)
    {
        asset_source = std::filesystem::path(argv[2]);
    }
    else
    {
        asset_source = std::filesystem::path("../assets/cube/cube.obj");
    }

    CameraSettings settings{};
    settings.m_image_width       = 800;
    settings.m_samples_per_pixel = 4;
    settings.m_ray_bounces       = 15;
    settings.m_vfow_deg          = 20;

    settings.m_look_from      = vec3(-8.0f, 0.0f, 0.5f);
    settings.m_look_at        = vec3(0.0f, 0.0f, 0.0f);
    settings.m_world_up       = vec3(0.0f, 1.0f, 0.0f);
    settings.m_defocus_angle  = 0.1;
    settings.m_focus_distance = 3.4;

    Camera camera(settings);

    u32   mesh_idx = meshes.size();
    Mesh& mesh     = meshes.emplace_back(import_mesh(asset_source));

    std::vector<std::shared_ptr<Triangle>> triangles;
    triangles.reserve(mesh.m_num_triangles);

    for (u32 t = 0; t < mesh.m_num_triangles; ++t)
    {
        triangles.emplace_back(std::make_shared<Triangle>(mesh_idx, t));
    }

    fmt::println("Triangle count in an imported mesh {}", triangles.size());

    std::shared_ptr<radiant::Sphere> ground_sphere = std::make_shared<Sphere>(vec3(0.0, -101, -1.0), 100.0);

    std::shared_ptr<Lambertian> material_ground = std::make_shared<Lambertian>(rgba(0.8f, 0.8f, 0.0f, 1.0));
    std::shared_ptr<Lambertian> material_center = std::make_shared<Lambertian>(rgba(0.1f, 0.2f, 0.5f, 1.0));
    std::shared_ptr<Dielectric> material_left   = std::make_shared<Dielectric>(1.0 / 1.33);
    std::shared_ptr<Metal>      material_right  = std::make_shared<Metal>(rgba(0.0, 66.0 / 256.0, 37.0 / 256.0, 1.0), 0.1f);
    std::shared_ptr<Lambertian> triangle_mat    = std::make_shared<Lambertian>(rgba(0.0, 66.0 / 256.0, 37.0 / 256.0, 1.0));

    std::vector<std::shared_ptr<Primitive>> triangle_prims;
    triangle_prims.reserve(triangles.size());

    for (const auto& triangle : triangles)
    {
        triangle_prims.push_back(std::make_shared<Primitive>(triangle, triangle_mat));
    }
    std::shared_ptr<Primitive> ground = std::make_shared<Primitive>(ground_sphere, material_ground);

    BVHAggregate aggregate;
    // Populate the scene
    aggregate.insert(ground.get());
    for (const auto& triangle_prim : triangle_prims)
    {
        aggregate.insert(triangle_prim.get());
    }

    aggregate.build();

    // Render the scene to the image buffer
    RenderTarget target{};
    camera.render(&aggregate, target);

    // TODO: Make collecting counters and stats more easy then ifdef
#ifndef NDEBUG
    fmt::println("{} total intersection calls ", aggregate.intersection_called_counter);
    fmt::println("{} primitive intersection calls ", aggregate.primitive_intersection_called);
    fmt::println("{} aabb intersection calls ", aggregate.aabb_intersection_called);
    assert(aggregate.intersection_called_counter ==
           aggregate.primitive_intersection_called + aggregate.aabb_intersection_called);
    fmt::println("Emitted {} primary rays", settings.m_image_height * settings.m_image_width * settings.m_samples_per_pixel);
    fmt::println("{} rays did not hit", aggregate.missed);

#endif

    fmt::println("Writing to {}", destination.string());
    garbage::write_png(target.render_target.data(), target.width, target.height, destination);

    return 0;
}
