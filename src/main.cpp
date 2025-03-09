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

rgb_color ray_color(const Ray& r, const Aggregate* aggregate)
{
    // This returns a "point of contact" along the ray
    std::optional<Intersection> intersection = aggregate->intersect(r, 0, inf);
    if (intersection)
    {
        vec3f     n = intersection->m_normal;
        rgb_color n_vis(n);
        n_vis += 1.0f;
        n_vis *= 0.5f;
        return n_vis;
    }

    vec3f udir  = normalized(r.m_direction);
    f32   blend = 0.5f * (udir[1] + 1.0);

    rgb_color result = (1.0f - blend) * rgb_color(1.0f, 1.0f, 1.0f) + blend * rgb_color(0.5f, 0.7f, 1.0f);
    return result;
}

int main()
{
    printf("%s", "Starting Radiant\n");

    // Set up the output image parameters, and allocate the image
    f32                    aspect_ratio = 16.0f / 9.0f;
    u32                    image_width  = 800;
    u32                    image_height = image_width / aspect_ratio;
    std::vector<rgb_color> image(image_width * image_height);

    // Set up camera parameters
    f32   focal_length    = 1.0f;
    f32   viewport_height = 2.0f;
    vec3f camera_pos      = vec3f(0.0f, 0.0f, 0.0f);

    // TODO: Figure out why is it done like this
    f32 viewport_width = viewport_height * static_cast<f32>(image_width) / image_height;

    // Calculate viewport spanning vector and viewport pixel deltas
    vec3f viewport_x(viewport_width, 0.0f, 0.0f);
    vec3f viewport_y(0, -viewport_height, 0);

    vec3f pixel_delta_x = viewport_x / image_width;
    vec3f pixel_delta_y = viewport_y / image_height;

    // TODO: Check this math
    vec3f viewport_origin = camera_pos - vec3f(0.0f, 0.0f, focal_length) - viewport_x / 2 - viewport_y / 2;
    vec3f start_pixel_pos = viewport_origin + 0.5f * (pixel_delta_x + pixel_delta_y);

    // Populate the scene

    Sphere          small(vec3f(0.0f, 0.0f, -1.0f), 0.5f);
    Sphere          big(vec3f(0.0f, -100.5f, -1.0f), 100.0f);
    LinearAggregate aggregate;
    aggregate.insert(&small);
    aggregate.insert(&big);

    // Render the scene to the image buffer
    printf("Rendering to image\n");
    for (u32 r = 0; r < image_height; ++r)
    {
        for (u32 c = 0; c < image_width; ++c)
        {
            vec3f pixel_sample_loc = start_pixel_pos + (r * pixel_delta_y) + (c * pixel_delta_x);

            // Ray is directed from the eye point through the sample point
            vec3f ray_direction = pixel_sample_loc - camera_pos;
            Ray   ray(camera_pos, ray_direction);
            image[c + r * image_width] = ray_color(ray, &aggregate);
        }
    }
    std::filesystem::path destination("test.png");
    garbage::write_png(image.data(), image_width, image_height, destination);

    return 0;
}