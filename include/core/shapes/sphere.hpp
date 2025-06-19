#pragma once

#include <core/shapes/shape.hpp>

namespace radiant
{
struct Sphere : public Shape
{
    Sphere(const vec3& center, Scalar radius);

    bool                               test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override;
    std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override;

    vec3   m_center{ 0.0 };
    Scalar m_radius{ 1.0 };
};
} // namespace radiant