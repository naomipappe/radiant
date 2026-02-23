#include "core/acceleration_structures/bvh_aggregate.hpp"
#include "core/camera.hpp"
#include "core/material.hpp"
#include "core/primitive.hpp"
#include "core/render_target.hpp"
#include "fmt/base.h"
#include "importers/obj.hpp"
#include <garbage/garbage_dump.hpp>

#include <core/vec.hpp>
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
    settings.m_samples_per_pixel = 50;
    settings.m_ray_bounces       = 25;
    settings.m_vfow_deg          = 20;

    settings.m_look_from      = vec3(-2.0f, 0.0f, 10.0f);
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

    std::shared_ptr<Sphere> ground_sphere = std::make_shared<Sphere>(vec3(0.0, -101, -1.0), 100.0);
    std::shared_ptr<Sphere> left_sphere = std::make_shared<Sphere>(vec3(-3.0, 1, 0.0), 2.0);
    std::shared_ptr<Sphere> light_sphere  = std::make_shared<Sphere>(vec3(3.2, 2, 0.0), 3);

    std::shared_ptr<Lambertian> material_ground = std::make_shared<Lambertian>(vec3(0.8f, 0.8f, 0.0f));
    std::shared_ptr<Lambertian> lambertian      = std::make_shared<Lambertian>(vec3(0.1f, 0.5f, 0.2f));
    std::shared_ptr<Lambertian> material_light  = std::make_shared<Lambertian>(vec3(1.0f, 1.0f, 1.0f));
    material_light->m_emissive                  = vec3(1.0f, 1.0f, 1.0f);
    std::shared_ptr<Dielectric> dielectric      = std::make_shared<Dielectric>(1.9);
    std::shared_ptr<Metal>      metal           = std::make_shared<Metal>(vec3(0.0, 66.0 / 256.0, 37.0 / 256.0), 0.6f);

    std::vector<std::shared_ptr<Primitive>> triangle_prims;
    triangle_prims.reserve(triangles.size());

    for (const auto& triangle : triangles)
    {
        triangle_prims.push_back(std::make_shared<Primitive>(triangle, lambertian));
    }

    std::shared_ptr<Primitive> ground = std::make_shared<Primitive>(ground_sphere, material_ground);
    std::shared_ptr<Primitive> left = std::make_shared<Primitive>(left_sphere, dielectric);
    std::shared_ptr<Primitive> light  = std::make_shared<Primitive>(light_sphere, material_light);

    BVHAggregate aggregate;
    // Populate the scene
    aggregate.insert(ground.get());
    aggregate.insert(left.get());
    aggregate.insert(light.get());

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
    fmt::println("{} primitive intersection calls ", aggregate.primitive_intersection_called);
    fmt::println("{} aabb intersection calls ", aggregate.aabb_test);

    fmt::println("Emitted {} primary rays", settings.m_image_height * settings.m_image_width * settings.m_samples_per_pixel);
    fmt::println("{} rays rejected by bounding box", aggregate.aabb_reject);
    fmt::println("{} rays missed", aggregate.missed);

#endif

    fmt::println("Writing to {}", destination.string());
    garbage::write_png(target.render_target.data(), target.width, target.height, destination);

    return 0;
}
