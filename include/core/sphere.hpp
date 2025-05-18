#pragma once

#include <core/primitive.hpp>

namespace radiant
{
class Sphere : public Primitive
{
  public:
    Sphere(const vec3& center, Scalar radius);
    Sphere(const vec3& center, Scalar radius, Material* material);

    bool                        test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override;
    std::optional<Intersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override;

  private:
    vec3      m_center{ 0.0 };
    Scalar    m_radius{ 1.0 };
    Material* m_material{ nullptr };
};
} // namespace radiant