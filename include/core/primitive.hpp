#pragma once

#include <memory>
#include <optional>

#include <core/vec.hpp>
#include <core/ray.hpp>
#include <core/surface_interaction.hpp>
#include <core/shapes/shape.hpp>
#include <core/material.hpp>

namespace radiant
{
// TODO: Should be able to transform a primitive

struct Primitive
{
    Primitive(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Material>& material) :
        m_shape(shape), m_material(material)
    {}

    [[nodiscard]] bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const
    {
        return m_shape->test_intersection(r, tmin, tmax);
    }

    [[nodiscard]] std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const
    {
        if (std::optional<SurfaceIntersection> intersection = m_shape->intersect(r, tmin, tmax))
        {
            intersection->m_material = m_material.get();
            return intersection;
        }
        return std::nullopt;
    }

    [[nodiscard]] vec3 aabb_min() const { return m_shape->aabb_min; }
    [[nodiscard]] vec3 aabb_max() const { return m_shape->aabb_max; }

    std::shared_ptr<Shape>    m_shape;
    std::shared_ptr<Material> m_material;
};

} // namespace radiant