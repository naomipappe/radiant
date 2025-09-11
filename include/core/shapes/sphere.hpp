#pragma once

#include <core/shapes/shape.hpp>

namespace radiant
{
struct Sphere : Shape
{
    Sphere(const vec3& center, Scalar radius);

    [[nodiscard]] bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override;
    [[nodiscard]] std::optional<Intersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override;
    Scalar                                           m_radius{ 1.0 };
};
} // namespace radiant