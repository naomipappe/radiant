#include "core/primitive.hpp"
#include "core/probability/sampling.hpp"
#include "core/ray.hpp"
#include "core/vec.hpp"
#include <core/material.hpp>
#include <optional>
namespace radiant
{

Lambertian::Lambertian(const radiant::rgb_color& albedo) : m_albedo(albedo) {}

std::optional<radiant::Ray>
Lambertian::scatter(const radiant::Ray& ray, const Intersection& intersection, radiant::rgb_color& attenuation) const
{
    vec3f scattering_direction = intersection.m_normal + random<f32, 3>().normalize();
    if (scattering_direction.is_zero())
    {
        scattering_direction = intersection.m_normal;
    }
    attenuation = m_albedo;
    return std::make_optional<Ray>(intersection.m_intersection, scattering_direction);
}

Metal::Metal(const rgb_color& albedo) : m_albedo(albedo) {}

std::optional<radiant::Ray>
Metal::scatter(const radiant::Ray& ray, const Intersection& intersection, radiant::rgb_color& attenuation) const
{
    vec3f reflected = reflect(ray.m_direction, intersection.m_normal);
    if (reflected.is_zero())
    {
        reflected = intersection.m_normal;
    }
    attenuation     = m_albedo;
    return std::make_optional<radiant::Ray>(intersection.m_intersection, reflected);
}

} // namespace radiant
