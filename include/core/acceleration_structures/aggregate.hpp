#pragma once

#include <core/primitive.hpp>

namespace radiant
{
class Aggregate
{
  public:
    virtual bool                        test_intersection(const Ray& r, f32 tmin, f32 tmax) const = 0;
    virtual std::optional<Intersection> intersect(const Ray& r, f32 tmin, f32 tmax) const         = 0;
    virtual void                        insert(Primitive* primitive)                              = 0;
    virtual void                        clear()                                                   = 0;
};

} // namespace radiant
