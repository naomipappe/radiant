#include "core/acceleration_structures/aggregate.hpp"
#include "core/color.hpp"
#include "core/constants.hpp"
#include "core/render_target.hpp"
#include <core/camera.hpp>

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
}

rgb_color Camera::ray_color(const Ray& ray, const Aggregate* aggregate)
{
    // This returns a "point of contact" along the ray
    std::optional<Intersection> intersection = aggregate->intersect(ray, 0, inf);
    if (intersection)
    {
        vec3f n = (intersection->m_normal + 1.0f) * 0.5f;
        return n;
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
    for (u32 r = 0; r < m_settings.m_image_height; ++r)
    {
        for (u32 c = 0; c < m_settings.m_image_width; ++c)
        {
            vec3f pixel_sample_loc =
                m_settings.m_pixel_00_loc + (r * m_settings.m_pixel_delta_v) + (c * m_settings.m_pixel_delta_u);

            // Ray is directed from the eye point through the sample point
            vec3f ray_direction = normalized(pixel_sample_loc - m_settings.m_position);
            Ray   ray(m_settings.m_position, ray_direction);
            render_target.render_target[c + r * m_settings.m_image_width] = ray_color(ray, aggregate);
        }
    }
}

} // namespace radiant