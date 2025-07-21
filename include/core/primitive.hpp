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
    virtual ~Primitive()                                                                                       = default;
    virtual bool                               test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const = 0;
    virtual std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const         = 0;
};

struct GeometricPrimitive : public Primitive
{
    GeometricPrimitive(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Material>& material) :
        m_shape(shape), m_material(material)
    {}

    virtual ~GeometricPrimitive() override = default;

    bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        return m_shape->test_intersection(r, tmin, tmax);
    }

    std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        std::optional<SurfaceIntersection> intersection = m_shape->intersect(r, tmin, tmax);
        if (intersection)
        {
            intersection->m_material = m_material.get();
            return intersection;
        }
        return std::nullopt;
    }
    virtual vec3 aabb_min() const { return m_shape->aabb_min; }
    virtual vec3 aabb_max() const { return m_shape->aabb_max; }

    std::shared_ptr<Shape>    m_shape;
    std::shared_ptr<Material> m_material;
};

} // namespace radiant