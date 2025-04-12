#pragma once

#include <core/primitive.hpp>

namespace radiant
{
class Sphere : public Primitive
{
  public:
    Sphere(const vec3f& center, f32 radius);
    Sphere(const vec3f& center, f32 radius, Material* material);

    bool                        test_intersection(const Ray& r, f32 tmin, f32 tmax) const override;
    std::optional<Intersection> intersect(const Ray& r, f32 tmin, f32 tmax) const override;

  private:
    vec3f     m_center{ 0.0f };
    f32       m_radius{ 1.0f };
    Material* m_material{ nullptr };
};
} // namespace radiant