#pragma once

#include "core/ray.hpp"
#include <optional>

// Rebuild with dependency inhection -> BxDF, Textures can be plugged in, configured during creation
// Look at PBRT for inspiration (or not)
namespace radiant
{
struct Material
{
    virtual ~Material() {};

    virtual Ray scatter(const Ray& ray, Intersection& intersection, vec3& attenuation) const = 0;
    vec3        m_emissive;
};

struct Lambertian : Material
{
    explicit Lambertian(const vec3& albedo);
    Ray scatter(const Ray& ray, Intersection& intersection, vec3& attenuation) const override;
    virtual ~Lambertian() {}

    vec3 m_albedo;
};

struct Metal : Material
{
    Metal(const vec3& albedo, Scalar roughness);

    Ray scatter(const Ray& ray, Intersection& intersection, vec3& attenuation) const override;
    virtual ~Metal() {}

    vec3   m_albedo; // Notice that both materials have albedo
    Scalar m_roughness;
};

struct Dielectric : Material
{

    explicit Dielectric(Scalar effective_refraction_index);

    Ray scatter(const Ray& ray, Intersection& intersection, vec3& attenuation) const override;
    virtual ~Dielectric() {};

    Scalar reflectance(Scalar cosine, Scalar refraction_index) const;

    Scalar m_effective_refraction_index;
};
} // namespace radiant