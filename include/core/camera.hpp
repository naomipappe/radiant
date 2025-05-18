#pragma once

#include "core/acceleration_structures/aggregate.hpp"
#include "core/color.hpp"
#include "core/ray.hpp"
#include "core/render_target.hpp"
#include "core/types.hpp"
#include "core/vec.hpp"

namespace radiant
{

// Split this into camera internal and external variables
// Something we should be able to set, something - not
struct CameraSettings
{
    Scalar m_aspect_ratio{ 16.0 / 9.0 };
    u32    m_image_width{ 800 };
    u32    m_image_height{ static_cast<u32>(m_image_width / m_aspect_ratio) };

    vec3 m_position{ 0, 0, 0 };
    vec3 m_pixel_00_loc{};
    vec3 m_pixel_delta_u{};
    vec3 m_pixel_delta_v{};

    u32    m_samples_per_pixel{ 10 };
    Scalar m_sampling_scale = 1.0 / m_samples_per_pixel;

    u32 m_ray_bounces{ 10 };

    Scalar m_vfow_deg{ 90 };
    vec3   m_look_from{ 0.0f, 0.0f, 0.0f };
    vec3   m_look_at{ 0.0f, 0.0f, -1.0f };
    vec3   m_world_up{ 0.0f, 1.0f, 0.0f };

    Scalar m_defocus_angle{ 0.0f };
    Scalar m_focus_distance{ 10.0f };
};

class Camera
{
  public:
    Camera();
    Camera(const CameraSettings& settings);

    void render(const Aggregate* aggregate, RenderTarget& render_target);

  private:
    void      init(const CameraSettings& settings);
    rgb_color ray_color(const Ray& ray, const Aggregate* aggregate, u32 bounce);
    vec3      sample_square() const;
    vec3      sample_defocus_disk() const;
    Ray       jittered_ray(u32 u, u32 v);

  private:
    CameraSettings m_settings{};

    // Internal camera frame basis
    vec3 m_u, m_v, m_w;

    vec3 m_defocus_disk_u{};
    vec3 m_defocus_disk_v{};
};

} // namespace radiant