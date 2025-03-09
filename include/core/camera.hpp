#pragma once

#include "core/acceleration_structures/aggregate.hpp"
#include "core/color.hpp"
#include "core/ray.hpp"
#include "core/render_target.hpp"
#include "core/types.hpp"
#include "core/vec.hpp"

namespace radiant
{
struct CameraSettings
{
    f32 m_aspect_ratio{ 16.0 / 9.0 };
    u32 m_image_width{ 800 };
    u32 m_image_height{ static_cast<u32>(m_image_width / m_aspect_ratio) };

    vec3f m_position{ 0, 0, 0 };
    vec3f m_pixel_00_loc{};
    vec3f m_pixel_delta_u{};
    vec3f m_pixel_delta_v{};

    f32 m_focal_length{ 1.0 };
};

class Camera
{
  public:
    CameraSettings m_settings{};

    Camera();
    Camera(const CameraSettings& settings);

    void render(const Aggregate* aggregate, RenderTarget& render_target);

  private:
    void      init(const CameraSettings& settings);
    rgb_color ray_color(const Ray& ray, const Aggregate* aggregate);
};

} // namespace radiant