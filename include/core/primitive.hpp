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
    Intersection(const vec3f& intersection, const vec3f& normal, f32 t, Material* material) :
        m_intersection(intersection), m_normal(normal), m_t(t), m_material(material)
    {}
    vec3f     m_intersection;
    vec3f     m_normal;
    f32       m_t;
    Material* m_material;
};

// TODO: Should be able to transform a primitive

class Primitive
{
  public:
    virtual ~Primitive()                                                                          = default;
    virtual bool                        test_intersection(const Ray& r, f32 tmin, f32 tmax) const = 0;
    virtual std::optional<Intersection> intersect(const Ray& r, f32 tmin, f32 tmax) const         = 0;
};
} // namespace radiant