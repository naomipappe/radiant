#include "core/camera.hpp"
#include "core/color.hpp"
#include "core/material.hpp"
#include "core/primitive.hpp"
#include "core/render_target.hpp"
#include "core/triangle_mesh.hpp"
#include "fmt/base.h"
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

int main()
{
    fmt::println("{}", "Starting Radiant");

    CameraSettings settings{};
    settings.m_image_width       = 800;
    settings.m_samples_per_pixel = 4;
    settings.m_ray_bounces       = 10;
    settings.m_vfow_deg          = 20;

    settings.m_look_from      = vec3(0.0f, 1.0f, 10.0f);
    settings.m_look_at        = vec3(0.0f, 0.0f, -1.0f);
    settings.m_world_up       = vec3(0.0f, 1.0f, 0.0f);
    settings.m_defocus_angle  = 0.1;
    settings.m_focus_distance = 3.4;

    Camera camera(settings);

    std::vector<vec3> positions = {
        vec3{ 0.0, 1.0, -1.0 },  // vertex 0: front
        vec3{ -1.0, 0.0, -1.0 }, // vertex 1: back-left
        vec3{ 1.0, 0.0, -1.0 },  // vertex 2: back-right
    };

    std::vector<u32>  indices      = { 0, 1, 2 };
    std::vector<vec3> per_vertex_n = {
        vec3{ 0.0, 0.0, 1.0 }, // vertex 0: front
        vec3{ 0.0, 0.0, 1.0 }, // vertex 1: back-left
        vec3{ 0.0, 0.0, 1.0 }, // vertex 2: back-right
    };

    std::vector<std::shared_ptr<StaticTriangleMesh>> global_meshes{};
    std::shared_ptr<StaticTriangleMesh>              mesh = std::make_shared<StaticTriangleMesh>();

    mesh->m_num_triangles = 1;
    mesh->m_num_vertices  = 3;
    mesh->m_indices       = indices.data();
    mesh->m_positions     = positions.data();
    mesh->m_normals       = per_vertex_n.data();

    Triangle::g_meshes = &global_meshes;
    Triangle::g_meshes->push_back(mesh);
    std::vector<std::shared_ptr<Triangle>> triangles;
    triangles.emplace_back(std::make_shared<Triangle>(0, 0));

    std::shared_ptr<Lambertian>         red_triangle_material = std::make_shared<Lambertian>(rgb_color(0.0, 66.0 / 256.0, 37.0 / 256.0));
    std::shared_ptr<GeometricPrimitive> face = std::make_shared<GeometricPrimitive>(triangles[0], red_triangle_material);

    // Populate the scene
    std::shared_ptr<Lambertian> material_ground = std::make_shared<Lambertian>(rgb_color(0.8f, 0.8f, 0.0f));
    std::shared_ptr<Lambertian> material_center = std::make_shared<Lambertian>(rgb_color(0.1f, 0.2f, 0.5f));
    std::shared_ptr<Dielectric> material_left   = std::make_shared<Dielectric>(1.0 / 1.33);
    std::shared_ptr<Metal>      material_right  = std::make_shared<Metal>(rgb_color(0.8f, 0.6f, 0.2f), 1.0f);

    std::shared_ptr<radiant::Sphere> ground_sphere = std::make_shared<radiant::Sphere>(vec3(0.0, -100.5, -1.0), 100.0);
    std::shared_ptr<radiant::Sphere> left_sphere   = std::make_shared<radiant::Sphere>(vec3(-1.1f, 0.0f, -1.0f), 0.5f);
    std::shared_ptr<radiant::Sphere> right_sphere  = std::make_shared<radiant::Sphere>(vec3(0.0f, 0.0f, -1.2f), 0.5f);
    std::shared_ptr<radiant::Sphere> center_sphere = std::make_shared<radiant::Sphere>(vec3(1.1f, 0.0f, -1.0f), 0.5f);

    std::shared_ptr<GeometricPrimitive> ground = std::make_shared<GeometricPrimitive>(ground_sphere, material_ground);
    std::shared_ptr<GeometricPrimitive> left   = std::make_shared<GeometricPrimitive>(left_sphere, material_left);
    std::shared_ptr<GeometricPrimitive> center = std::make_shared<GeometricPrimitive>(center_sphere, material_center);
    std::shared_ptr<GeometricPrimitive> right  = std::make_shared<GeometricPrimitive>(right_sphere, material_right);

    LinearAggregate aggregate;
    // aggregate.insert(left.get());
    // aggregate.insert(center.get());
    // aggregate.insert(right.get());
    aggregate.insert(ground.get());
    aggregate.insert(face.get());

    // Render the scene to the image buffer
    RenderTarget target{};
    camera.render(&aggregate, target);

    std::filesystem::path destination("test.png");
    garbage::write_png(target.render_target.data(), target.width, target.height, destination);

    return 0;
}