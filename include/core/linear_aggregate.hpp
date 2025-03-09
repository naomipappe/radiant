#pragma once

#include <span>
#include <vector>
#include <core/aggregate.hpp>

namespace radiant
{
class LinearAggregate : public Aggregate
{
  public:
    LinearAggregate() = default;
    explicit LinearAggregate(const std::span<Primitive*>& primitives);
    ~LinearAggregate() = default;

    virtual bool                        test_intersection(const Ray& r, f32 tmin, f32 tmax) const override;
    virtual std::optional<Intersection> intersect(const Ray& r, f32 tmin, f32 tmax) const override;
    virtual void                        insert(Primitive* primitive) override;
    virtual void                        clear() override;

  private:
    std::vector<Primitive*> m_primitives;
};
} // namespace radiant
