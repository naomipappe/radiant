#include <cmath>
#include <core/primitive.hpp>
#include <core/probability/sampling.hpp>
#include <core/ray.hpp>
#include <core/types.hpp>
#include <core/vec.hpp>
#include <core/material.hpp>
#include <optional>
namespace radiant
{

Lambertian::Lambertian(const vec3& albedo) : m_albedo(albedo) {}

Ray Lambertian::scatter(const Ray& ray, Intersection& intersection, vec3& attenuation) const
{
    vec3 scattering_direction = intersection.m_normal + random<Scalar, 3>().normalize();
    if (scattering_direction.is_zero())
    {
        scattering_direction = intersection.m_normal;
    }
    attenuation = m_albedo;
    return { intersection.m_intersection, normalized(scattering_direction) };
}

Metal::Metal(const vec3& albedo, Scalar roughness) : m_albedo(albedo), m_roughness(std::clamp(roughness, 0.0, 1.0)) {}

Ray Metal::scatter(const Ray& ray, Intersection& intersection, vec3& attenuation) const
{
    vec3 reflected = reflect(ray.m_direction, intersection.m_normal).normalize();
    reflected += sample_sphere_hemisphere_rejection(intersection.m_normal) * m_roughness;
    attenuation = m_albedo;
    return { intersection.m_intersection, normalized(reflected) };
}

Dielectric::Dielectric(Scalar effective_refraction_index) : m_effective_refraction_index(effective_refraction_index) {}

Ray Dielectric::scatter(const Ray& ray, Intersection& intersection, vec3& attenuation) const
{
    attenuation = ones<Scalar, 3>();
    Scalar ri   = m_effective_refraction_index;
    if (dot(ray.m_direction, intersection.m_normal) > 0)
    {
        intersection.m_normal = -intersection.m_normal;
    }
    else
    {
        ri = 1.0 / ri;
    }

    vec3   unit_direction = normalized(ray.m_direction);
    Scalar cos_theta      = std::fmin(dot(-unit_direction, intersection.m_normal), 1.0);
    Scalar sin_theta      = std::sqrt(1 - cos_theta * cos_theta);

    vec3 direction;
    if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random<Scalar>())
    {
        direction = reflect(unit_direction, intersection.m_normal);
    }
    else
    {
        direction = refract(unit_direction, intersection.m_normal, ri);
    }
    return { intersection.m_intersection, normalized(direction) };
}

Scalar Dielectric::reflectance(Scalar cosine, Scalar refraction_index) const
{
    Scalar r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 *= r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}

} // namespace radiant
