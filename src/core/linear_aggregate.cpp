#include <core/acceleration_structures/linear_aggregate.hpp>

namespace radiant
{
LinearAggregate::LinearAggregate(const std::span<Primitive*>& primitives) :
    m_primitives(primitives.begin(), primitives.end())
{}

bool LinearAggregate::test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const
{
    return intersect(r, tmin, tmax) != std::nullopt;
}
std::optional<Intersection> LinearAggregate::intersect(const Ray& r, Scalar tmin, Scalar tmax) const
{
    std::optional<Intersection> intersection = std::nullopt;
    Scalar                         closest      = tmax;

    std::optional<Intersection> iterator;
    for (const Primitive* primitive : m_primitives)
    {
        iterator = primitive->intersect(r, tmin, tmax);
        if (iterator && iterator->m_t < closest)
        {
            closest      = iterator->m_t;
            intersection = iterator;
        }
    }
    return intersection;
}

void LinearAggregate::insert(Primitive* primitive)
{
    m_primitives.push_back(primitive);
}

void LinearAggregate::clear()
{
    m_primitives.clear();
}
} // namespace radiant
