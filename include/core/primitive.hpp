#pragma once

#include <optional>

#include <core/vec.hpp>
#include <core/ray.hpp>

namespace radiant
{

class Material;
// The decision here is that we always use the dot product to determine whether the ray comes from inside or not
struct Intersection
{
    Intersection() = default;
    Intersection(const vec3& intersection, const vec3& normal, Scalar t, Material* material) :
        m_intersection(intersection), m_normal(normal), m_t(t), m_material(material)
    {}
    vec3     m_intersection;
    vec3     m_normal;
    Scalar       m_t;
    Material* m_material;
};

// TODO: Should be able to transform a primitive

class Primitive
{
  public:
    virtual ~Primitive()                                                                          = default;
    virtual bool                        test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const = 0;
    virtual std::optional<Intersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const         = 0;
};
} // namespace radiant