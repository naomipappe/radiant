#include <core/primitive.hpp>
#include <core/probability/sampling.hpp>
#include <core/ray.hpp>
#include <core/types.hpp>
#include <core/vec.hpp>
#include <fmt/base.h>
#include <fmt/ranges.h>
#include <core/material.hpp>
#include <optional>
namespace radiant
{

Lambertian::Lambertian(const rgb_color& albedo) : m_albedo(albedo) {}

std::optional<Ray> Lambertian::scatter(const Ray& ray, Intersection& intersection, rgb_color& attenuation) const
{
    vec3f scattering_direction = intersection.m_normal + random<f32, 3>().normalize();
    if (scattering_direction.is_zero())
    {
        scattering_direction = intersection.m_normal;
    }
    attenuation = m_albedo;
    return std::make_optional<Ray>(intersection.m_intersection, scattering_direction);
}

Metal::Metal(const rgb_color& albedo, f32 roughness) : m_albedo(albedo), m_roughness(std::clamp(roughness, 0.0f, 1.0f)) {}

std::optional<Ray> Metal::scatter(const Ray& ray, Intersection& intersection, rgb_color& attenuation) const
{
    vec3f reflected = reflect(ray.m_direction, intersection.m_normal).normalize();
    reflected += random<f32, 3>() * m_roughness;
    attenuation = m_albedo;
    return std::make_optional<Ray>(intersection.m_intersection, reflected);
}

Dielectric::Dielectric(Scalar effective_refraction_index) : m_effective_refraction_index(effective_refraction_index) {}

std::optional<Ray> Dielectric::scatter(const Ray& ray, Intersection& intersection, rgb_color& attenuation) const
{
    attenuation = ones<Scalar, 3>();
    Scalar ri   = m_effective_refraction_index;
    if (dot(ray.m_direction, intersection.m_normal) < 0)
    {
        ri                    = 1.0 / ri;
        intersection.m_normal = -intersection.m_normal;
    }

    vec3f refracted = refract(normalized(ray.m_direction), intersection.m_normal, ri);
    return std::make_optional<Ray>(intersection.m_intersection, refracted);
}

} // namespace radiant
