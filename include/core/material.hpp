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

    virtual std::optional<radiant::Ray>
    scatter(const radiant::Ray& ray, const Intersection& intersection, radiant::rgb_color& attenuation) const
    {
        return std::nullopt;
    }
};

class Lambertian : public Material
{
  public:
    explicit Lambertian(const radiant::rgb_color& albedo);
    std::optional<radiant::Ray>
    scatter(const radiant::Ray& ray, const Intersection& intersection, radiant::rgb_color& attenuation) const override;
    virtual ~Lambertian() {};

  private:
    radiant::rgb_color m_albedo;
};

class Metal : public Material
{
  public:
    Metal(const rgb_color& albedo);

    std::optional<radiant::Ray>
    scatter(const radiant::Ray& ray, const Intersection& intersection, radiant::rgb_color& attenuation) const override;
    virtual ~Metal() {};

  private:
    rgb_color m_albedo; // Notice that both materials have albedo
};
}; // namespace radiant