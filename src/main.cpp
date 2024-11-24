#include <cstdio>
#include <garbage/garbage_dump.hpp>

#include <core/ray.hpp>
#include <core/vec.hpp>

#include <filesystem>
#include <vector> // TODO: Remove STL when bored

using namespace radiant;

rgb_color ray_color(const Ray& r)
{
    vec3f udir = normalized(r.m_direction);
    f32   t    = 0.5f * (udir[1] + 1.0);
    rgb_color result =  (1.0f - t) * rgb_color(1.0, 1.0, 1.0) + t * rgb_color(0.5, 0.7, 1.0);
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
    f32   viewport_height = 1.0f;
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

    // Render the scene to the image buffer
    for (u32 r = 0; r < image_height; ++r)
    {
        for (u32 c = 0; c < image_width; ++c)
        {
            vec3f pixel_sample_loc = start_pixel_pos + (r * pixel_delta_y) + (c * pixel_delta_x);

            // Ray is directed from the eye point through the sample point
            vec3f ray_direction = pixel_sample_loc - camera_pos;
            Ray   ray(camera_pos, ray_direction);
            image[c + r * image_width] = ray_color(ray);
            ;
        }
    }

    std::filesystem::path destination("test.ppm");

    garbage::write_ppm(image.data(), image_width, image_height, destination);

    return 0;
}