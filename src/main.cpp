#include "core/acceleration_structures/bvh_aggregate.hpp"
#include "core/camera.hpp"
#include "core/color.hpp"
#include "core/material.hpp"
#include "core/primitive.hpp"
#include "core/render_target.hpp"
#include "core/triangle_mesh.hpp"
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
    if (argc == 2)
    {
        destination = std::filesystem::path(argv[1]);
    }
    else
    {
        destination = std::filesystem::current_path().parent_path() / "render.png";
    }

    CameraSettings settings{};
    settings.m_image_width       = 800;
    settings.m_samples_per_pixel = 4;
    settings.m_ray_bounces       = 5;
    settings.m_vfow_deg          = 20;

    settings.m_look_from      = vec3(10.0f, 0.0f, 10.0f);
    settings.m_look_at        = vec3(0.0f, 0.0f, 0.0f);
    settings.m_world_up       = vec3(0.0f, 1.0f, 0.0f);
    settings.m_defocus_angle  = 0.1;
    settings.m_focus_distance = 3.4;

    Camera camera(settings);

    // BAD
    std::vector<std::shared_ptr<StaticTriangleMesh>> global_meshes{};
    Triangle::g_meshes = &global_meshes;

    auto [mesh, triangles] = import_mesh("../assets/cube/cube.obj");

    std::shared_ptr<radiant::Sphere> ground_sphere = std::make_shared<radiant::Sphere>(vec3(0.0, -101, -1.0), 100.0);
    std::shared_ptr<radiant::Sphere> left_sphere   = std::make_shared<radiant::Sphere>(vec3(-1.1f, 0.0f, -1.0f), 0.5f);
    std::shared_ptr<radiant::Sphere> right_sphere  = std::make_shared<radiant::Sphere>(vec3(0.0f, 0.0f, -1.2f), 0.5f);
    std::shared_ptr<radiant::Sphere> center_sphere = std::make_shared<radiant::Sphere>(vec3(0.0f, 1.2f, 0.0f), 0.5f);

    std::shared_ptr<Lambertian> material_ground = std::make_shared<Lambertian>(rgb_color(0.8f, 0.8f, 0.0f));
    std::shared_ptr<Lambertian> material_center = std::make_shared<Lambertian>(rgb_color(0.1f, 0.2f, 0.5f));
    std::shared_ptr<Dielectric> material_left   = std::make_shared<Dielectric>(1.0 / 1.33);
    std::shared_ptr<Metal>      material_right  = std::make_shared<Metal>(rgb_color(0.8f, 0.6f, 0.2f), 0.3f);
    std::shared_ptr<Lambertian> triangle_mat    = std::make_shared<Lambertian>(rgb_color(0.0, 66.0 / 256.0, 37.0 / 256.0));

    std::vector<std::shared_ptr<GeometricPrimitive>> triangle_prims;
    triangle_prims.reserve(triangles.size());

    for (const auto& triangle : triangles)
    {
        triangle_prims.push_back(std::make_shared<GeometricPrimitive>(triangle, triangle_mat));
    }
    std::shared_ptr<GeometricPrimitive> ground = std::make_shared<GeometricPrimitive>(ground_sphere, material_ground);
    std::shared_ptr<GeometricPrimitive> center = std::make_shared<GeometricPrimitive>(center_sphere, material_right);

    BVHAggregate aggregate;
    // Populate the scene
    aggregate.insert(center.get());
    aggregate.insert(ground.get());
    for (const auto& triangle_prim : triangle_prims)
    {
        aggregate.insert(triangle_prim.get());
    }

    aggregate.build();

    // Render the scene to the image buffer
    RenderTarget target{};
    camera.render(&aggregate, target);

    fmt::println("Writing to {}", destination.string());
    garbage::write_png(target.render_target.data(), target.width, target.height, destination);

    return 0;
}