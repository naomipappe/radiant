#pragma once

#include "ray.hpp"

#include <limits>
#include "core/types.hpp"
#include "core/vec.hpp"

namespace radiant
{
const Scalar inf  = std::numeric_limits<Scalar>::infinity();
const Scalar eps  = std::numeric_limits<Scalar>::epsilon();
const Scalar PI   = 3.1415926535897932385;
const Scalar PI_2 = PI / 2;
const Scalar PI_3 = PI / 3;
const Scalar PI_4 = PI / 4;
const Scalar PI_6 = PI / 6;

Scalar deg_to_rad(Scalar degrees);
Scalar rad_to_get(Scalar radians);

Scalar det(const vec3& v1, const vec3& v2, const vec3& v3);

vec3 pointwise_min(const vec3& a, const vec3& b);
vec3 pointwise_max(const vec3& a, const vec3& b);

bool test_intersection_aabb(const Ray& r, vec3 aabb_min, vec3 aabb_max);

} // namespace radiant