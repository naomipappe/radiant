#pragma once
#include <optional>
#include <cassert>

#include <core/intersection.hpp>
#include <core/ray.hpp>

namespace radiant
{

enum class ShapeType : u32
{
    Sphere = 0,
    Triangle,
    Quad,
    NUM_SHAPES,
};

struct Shape;

Shape                       make_sphere(const vec3& position, float radius);
std::optional<Intersection> intersect_sphere(const Shape& s, const Ray& r, Scalar tmin, Scalar tmax);

Shape                       make_triangle(u64 mesh_index, u64 triangle_index);
std::optional<Intersection> intersect_triangle(const Shape& s, const Ray& r, Scalar tmin, Scalar tmax);

Shape                       make_quad(const vec3& position, const vec3& normal);
std::optional<Intersection> intersect_quad(const Shape& s, const Ray& r, Scalar tmin, Scalar tmax);

struct Shape
{
    [[nodiscard]] bool test_intersection(const Ray& r, const Scalar tmin, const Scalar tmax) const
    {
        const std::optional<Intersection> intersection = intersect(r, tmin, tmax);
        return intersection.has_value();
    }
    [[nodiscard]] std::optional<Intersection> intersect(const Ray& r, const Scalar tmin, const Scalar tmax) const
    {
        switch (m_type)
        {
            case ShapeType::Sphere:
            {
                return intersect_sphere(*this, r, tmin, tmax);
            }
            case ShapeType::Triangle:
            {
                return intersect_triangle(*this, r, tmin, tmax);
            }
            case ShapeType::Quad:
            {
                return std::nullopt;
            }
            default:
            {
                assert(false);
                return std::nullopt;
            }
        }
    }

    ShapeType m_type;
    union
    {
        struct
        {
            float radius;
        } sphere;
        struct
        {
            u64 mesh_index, triangle_index;
        } triangle;
    };

    vec3 aabb_min{ std::numeric_limits<Scalar>::max() };
    vec3 aabb_max{ -std::numeric_limits<Scalar>::max() };

    vec3 m_centroid;
};
} // namespace radiant