#pragma once

#include <core/primitive.hpp>

namespace radiant
{
class Aggregate
{
  public:
    virtual ~Aggregate()                                                                                       = default;
    virtual bool                        test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const = 0;
    virtual std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const         = 0;
    virtual void                        insert(Primitive* primitive)                              = 0;
    virtual void                        clear()                                                   = 0;
};

} // namespace radiant
