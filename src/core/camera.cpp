#include "core/acceleration_structures/aggregate.hpp"
#include "core/math.hpp"
#include "core/ray.hpp"
#include "core/render_target.hpp"
#include "core/types.hpp"
#include "core/vec.hpp"
#include "core/material.hpp"
#include "fmt/base.h"
#include <cassert>
#include <cmath>
#include <core/camera.hpp>

#include <core/probability/sampling.hpp>
#include <optional>

#include <omp.h>

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
    m_settings.m_image_height   = std::max(1u, u32(m_settings.m_image_width / m_settings.m_aspect_ratio));
    m_settings.m_sampling_scale = 1.0 / m_settings.m_samples_per_pixel;

    m_settings.m_position = m_settings.m_look_from;

    Scalar viewport_height = 2.0 * settings.m_focus_distance * std::tan(deg_to_rad(settings.m_vfow_deg) / 2);
    // TODO: Figure out why is it done like this
    Scalar viewport_width = viewport_height * static_cast<Scalar>(m_settings.m_image_width) / m_settings.m_image_height;

    m_w = normalized(m_settings.m_look_from - m_settings.m_look_at);
    m_u = normalized(cross(m_settings.m_world_up, m_w));
    m_v = cross(m_w, m_u);
    // Calculate viewport spanning vector and viewport pixel deltas
    vec3 viewport_u = viewport_width * m_u;
    vec3 viewport_v = -m_v * viewport_height;

    m_settings.m_pixel_delta_u = viewport_u / m_settings.m_image_width;
    m_settings.m_pixel_delta_v = viewport_v / m_settings.m_image_height;

    // TODO: Check this math
    vec3 viewport_origin = m_settings.m_position - (m_settings.m_focus_distance * m_w) - viewport_u / 2 - viewport_v / 2;
    m_settings.m_pixel_00_loc = viewport_origin + 0.5 * (m_settings.m_pixel_delta_u + m_settings.m_pixel_delta_v);

    Scalar defocus_radius = m_settings.m_focus_distance * std::tan(deg_to_rad(m_settings.m_defocus_angle / 2));
    m_defocus_disk_u      = m_u * defocus_radius;
    m_defocus_disk_v      = m_v * defocus_radius;
}

vec3 Camera::trace(const Ray& ray, const Aggregate* aggregate)
{
    vec3 L          = zeros<Scalar, 3>();
    vec3 throughput = ones<Scalar, 3>();
    Ray  r          = ray;
    for (uint32_t i = 0; i < m_settings.m_ray_bounces; ++i)
    {
        if (auto intersect = aggregate->intersect(r, 1e-3, inf))
        {
            L += throughput * intersect->m_material->m_emissive;
            vec3 color_mat;
            r = intersect->m_material->scatter(r, intersect.value(), color_mat);
            throughput *= color_mat;
        }
        else
        {
            L += throughput * vec3(0.1f, 0.1f, 0.1f);
            break;
        }
    }
    return L;
}

void Camera::render(const Aggregate* aggregate, RenderTarget& render_target)
{
    assert(render_target.render_target.size() == m_settings.m_image_height * m_settings.m_image_width);

    #pragma omp parallel for schedule(dynamic, 8) collapse(2)
    for (i32 v = 0; v < m_settings.m_image_height; ++v)
    {
        for (i32 u = 0; u < m_settings.m_image_width; ++u)
        {
            vec3 sampled_color = zeros<Scalar, 3>();
            for (u32 sample = 0; sample < m_settings.m_samples_per_pixel; ++sample)
            {
                sampled_color += trace(jittered_ray(u, v), aggregate);
            }
            sampled_color *= m_settings.m_sampling_scale;

            // Accumulate into float buffer (no quantization loss)
            auto & current = render_target.render_target[u + v * m_settings.m_image_width];
            current += (sampled_color - current) / float(render_target.frame);
        }
    }
}

vec3 Camera::sample_square() const
{
    return vec3(random(-0.5, 0.5), random(-0.5, 0.5), 0.0);
}

Ray Camera::jittered_ray(u32 u, u32 v)
{
    vec3 jitter_offset = sample_square();

    vec3 sample_loc = m_settings.m_pixel_00_loc + ((u + jitter_offset[0]) * m_settings.m_pixel_delta_u) +
                      ((v + jitter_offset[1]) * m_settings.m_pixel_delta_v);
    vec3 ray_direction = normalized(sample_loc - m_settings.m_position);
    vec3 ray_origin = vec3(m_settings.m_defocus_angle <= 0.0 ? m_settings.m_position.m_data : sample_defocus_disk().m_data);
    return Ray(ray_origin, ray_direction);
}

vec3 Camera::sample_defocus_disk() const
{
    vec3 p = sample_unit_disk();
    return m_settings.m_position + (p[0] * m_defocus_disk_u) + (p[1] * m_defocus_disk_v);
}

} // namespace radiant
