#include "core/camera.hpp"
#include "core/color.hpp"
#include "core/render_target.hpp"
#include <cstdio>
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
    printf("%s", "Starting Radiant\n");

    Camera camera;

    // Populate the scene
    Sphere          small(vec3f(0.0f, 0.0f, -1.0f), 0.5f);
    Sphere          big(vec3f(0.0f, -100.5f, -1.0f), 100.0f);
    LinearAggregate aggregate;
    aggregate.insert(&small);
    aggregate.insert(&big);

    // Render the scene to the image buffer
    RenderTarget target{};
    camera.render(&aggregate, target);

    std::filesystem::path destination("test.jpg");
    garbage::write_jpg(target.render_target.data(), target.width, target.height, destination);

    return 0;
}