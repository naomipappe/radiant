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

    virtual std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgb_color& attenuation) const
    {
        return std::nullopt;
    }
};

class Lambertian : public Material
{
  public:
    explicit Lambertian(const rgb_color& albedo);
    std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgb_color& attenuation) const override;
    virtual ~Lambertian() {};

  private:
    rgb_color m_albedo;
};

class Metal : public Material
{
  public:
    Metal(const rgb_color& albedo, f32 roughness);

    std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgb_color& attenuation) const override;
    virtual ~Metal() {};

  private:
    rgb_color m_albedo; // Notice that both materials have albedo
    f32       m_roughness;
};

class Dielectric : public Material
{
  public:
    explicit Dielectric(Scalar effective_refraction_index);

    std::optional<Ray> scatter(const Ray& ray, Intersection& intersection, rgb_color& attenuation) const override;
    virtual ~Dielectric() {};

  private:
    f32 reflectance(f32 cosine, f32 refraction_index) const;

  private:
    Scalar m_effective_refraction_index;
};
}; // namespace radiant