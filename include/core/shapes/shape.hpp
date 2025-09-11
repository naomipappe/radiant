#pragma once

#include <optional>

#include <core/intersection.hpp>
#include <core/ray.hpp>

namespace radiant
{
struct Shape
{
    virtual ~Shape()                                                                           = default;
    [[nodiscard]] virtual bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const = 0;
    [[nodiscard]] virtual std::optional<Intersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const = 0;

    vec3 aabb_min{ std::numeric_limits<Scalar>::max() };
    vec3 aabb_max{ -std::numeric_limits<Scalar>::max() };

    vec3 m_centroid;
};
} // namespace radiant