#pragma once

#include <optional>

#include <core/surface_interaction.hpp>
#include <core/ray.hpp>

namespace radiant
{
struct Shape
{
    virtual bool                               test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const = 0;
    virtual std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const         = 0;
};
} // namespace radiant