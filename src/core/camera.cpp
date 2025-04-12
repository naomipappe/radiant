#include "core/acceleration_structures/aggregate.hpp"
#include "core/color.hpp"
#include "core/constants.hpp"
#include "core/ray.hpp"
#include "core/render_target.hpp"
#include "core/types.hpp"
#include "core/vec.hpp"
#include <core/camera.hpp>

#include <logging/logging.hpp>

#include <core/probability/sampling.hpp>

namespace radiant
{
Camera::Camera()
{
    init(m_settings);
}
Camera::Camera(const CameraSettings& settings) : m_settings(settings)
{
    init(m_settings);
}

void Camera::init(const CameraSettings& settings)
{
    f32 viewport_height{ 2.0 };
    // TODO: Figure out why is it done like this
    f32 viewport_width = viewport_height * m_settings.m_image_width / m_settings.m_image_height;

    // Calculate viewport spanning vector and viewport pixel deltas
    vec3f viewport_u(viewport_width, 0.0f, 0.0f);
    vec3f viewport_v(0, -viewport_height, 0);

    m_settings.m_pixel_delta_u = viewport_u / m_settings.m_image_width;
    m_settings.m_pixel_delta_v = viewport_v / m_settings.m_image_height;

    // TODO: Check this math
    vec3f viewport_origin =
        m_settings.m_position - vec3f(0.0f, 0.0f, m_settings.m_focal_length) - viewport_u / 2 - viewport_v / 2;
    m_settings.m_pixel_00_loc = viewport_origin + 0.5f * (m_settings.m_pixel_delta_u + m_settings.m_pixel_delta_v);

    m_settings.m_sampling_scale = 1.0f / m_settings.m_samples_per_pixel;
}

rgb_color Camera::ray_color(const Ray& ray, const Aggregate* aggregate, u32 bounce, u32 bounces)
{
    if (bounce >= bounces)
    {
        return rgb_color(0.0f);
    }

    std::optional<Intersection> intersection = aggregate->intersect(ray, 1e-3f, inf);
    if (intersection)
    {
        vec3f reflected = intersection->m_normal + random_vec<f32,3>().normalize();
        return 0.5f * ray_color(Ray(intersection->m_intersection, reflected), aggregate, bounce + 1);
    }

    vec3f udir  = normalized(ray.m_direction);
    f32   blend = 0.5f * (udir[1] + 1.0);

    rgb_color result = (1.0f - blend) * rgb_color(1.0f, 1.0f, 1.0f) + blend * rgb_color(0.5f, 0.7f, 1.0f);
    return result;
}

void Camera::render(const Aggregate* aggregate, RenderTarget& render_target)
{
    if (render_target.render_target.size() < m_settings.m_image_height * m_settings.m_image_width)
    {
        render_target.render_target.resize(m_settings.m_image_height * m_settings.m_image_width, {});
        render_target.width  = m_settings.m_image_width;
        render_target.height = m_settings.m_image_height;
    }

    printf("Rendering to image\n");
    for (u32 v = 0; v < m_settings.m_image_height; ++v)
    {
        for (u32 u = 0; u < m_settings.m_image_width; ++u)
        {
            rgb_color sampled_color(0.0f, 0.0f, 0.0f);
            for (u32 sample = 0; sample < m_settings.m_samples_per_pixel; ++sample)
            {
                // Jitter the ray around the pixel center
                sampled_color += ray_color(jittered_ray(u, v), aggregate, 0);
            }
            render_target.render_target[u + v * m_settings.m_image_width] = sampled_color * m_settings.m_sampling_scale;
        }
    }
}

vec3f Camera::sample_square() const
{
    return vec3f(random(-0.5f, 0.5f), random(-0.5f, 0.5f), 0.0f);
}

Ray Camera::jittered_ray(u32 u, u32 v)
{
    vec3f jitter_offset = sample_square();

    vec3f sample_loc = m_settings.m_pixel_00_loc + ((u + jitter_offset[0]) * m_settings.m_pixel_delta_u) +
                       ((v + jitter_offset[1]) * m_settings.m_pixel_delta_v);
    vec3f ray_direction = normalized(sample_loc - m_settings.m_position);
    return Ray(m_settings.m_position, ray_direction);
}

} // namespace radiant