#pragma once

#include "core/primitive.hpp"
#include <span>
#include <vector>

#include <core/acceleration_structures/aggregate.hpp>

namespace radiant
{
class LinearAggregate : public Aggregate
{
  public:
    LinearAggregate() = default;
    explicit LinearAggregate(const std::span<Primitive*>& primitives);
    ~LinearAggregate() override = default;

    bool                               test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override;
    std::optional<Intersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override;
    void                               insert(Primitive* primitive) override;
    void                               clear() override;

  private:
    std::vector<Primitive*> m_primitives;
};
} // namespace radiant
