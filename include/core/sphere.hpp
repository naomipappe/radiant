#pragma once

#include <core/primitive.hpp>

namespace radiant
{
class Sphere : public Primitive
{
  public:
    Sphere(const vec3f& center, f32 radius);
    bool                        test_intersection(const Ray& r, f32 tmin, f32 tmax) const override;
    std::optional<Intersection> intersect(const Ray& r, f32 tmin, f32 tmax) const override;

  private:
    vec3f m_center;
    f32 m_radius;
};
} // namespace radiant