#include "core/camera.hpp"
#include "core/color.hpp"
#include "core/material.hpp"
#include "core/math.hpp"
#include "core/render_target.hpp"
#include "fmt/base.h"
#include <garbage/garbage_dump.hpp>

#include <core/constants.hpp>
#include <core/ray.hpp>
#include <core/vec.hpp>
#include <core/sphere.hpp>
#include <core/acceleration_structures/linear_aggregate.hpp>

#include <filesystem>
#include <vector> // TODO: Remove STL when bored

using namespace radiant;

int main()
{
    fmt::println("{}", "Starting Radiant");

    CameraSettings settings{};
    settings.m_samples_per_pixel = 100;
    settings.m_ray_bounces       = 50;
    settings.m_vfow_deg          = 20;

    settings.m_look_from      = vec3f(-2.0f, 2.0f, 1.0f);
    settings.m_look_at        = vec3f(0.0f, 0.0f, -1.0f);
    settings.m_world_up       = vec3f(0.0f, 1.0f, 0.0f);
    settings.m_defocus_angle  = 1.0f;
    settings.m_focus_distance = 3.4f;

    Camera camera(settings);

    // Populate the scene
    Lambertian material_ground(rgb_color(0.8f, 0.8f, 0.0f));
    Lambertian material_center(rgb_color(0.1f, 0.2f, 0.5f));
    Dielectric material_left  = Dielectric(1.0 / 1.33);
    Metal      material_right = Metal(rgb_color(0.8f, 0.6f, 0.2f), 1.0f);

    Sphere ground(vec3f(0.0f, -100.5f, -1.0f), 100.0f, &material_ground);
    Sphere left(vec3f(-1.1f, 0.0f, -1.0f), 0.5f, &material_left);
    Sphere center(vec3f(0.0f, 0.0f, -1.2f), 0.5f, &material_center);
    Sphere right(vec3f(1.1f, 0.0f, -1.0f), 0.5f, &material_right);

    LinearAggregate aggregate;
    aggregate.insert(&left);
    aggregate.insert(&center);
    aggregate.insert(&right);
    aggregate.insert(&ground);

    // Render the scene to the image buffer
    RenderTarget target{};
    camera.render(&aggregate, target);

    std::filesystem::path destination("test.png");
    garbage::write_png(target.render_target.data(), target.width, target.height, destination);

    return 0;
}