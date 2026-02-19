#pragma once

#include "core/color.hpp"
#include "core/ray.hpp"
#include <core/primitive.hpp>
#include <optional>

// Rebuild with dependency inhection -> BxDF, Textures can be plugged in, configured during creation
// Look at PBRT for inspiration (or not)
namespace radiant
{
class Material
{
  public:
    virtual ~Material() {};

    virtual std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgba& attenuation) const
    {
        return std::nullopt;
    }
};

class Lambertian : public Material
{
  public:
    explicit Lambertian(const rgba& albedo);
    std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgba& attenuation) const override;
    virtual ~Lambertian() {}

  private:
    rgba m_albedo;
};

class Metal : public Material
{
  public:
    Metal(const rgba& albedo, Scalar roughness);

    std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgba& attenuation) const override;
    virtual ~Metal() {}

  private:
    rgba m_albedo; // Notice that both materials have albedo
    Scalar    m_roughness;
};

class Dielectric : public Material
{
  public:
    explicit Dielectric(Scalar effective_refraction_index);

    std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgba& attenuation) const override;
    virtual ~Dielectric() {};

  private:
    Scalar reflectance(Scalar cosine, Scalar refraction_index) const;

  private:
    Scalar m_effective_refraction_index;
};
} // namespace radiant
