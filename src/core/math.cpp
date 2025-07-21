#include <core/math.hpp>

namespace radiant
{
Scalar deg_to_rad(Scalar degrees)
{
    return degrees * PI / 180.0f;
}
Scalar rad_to_get(Scalar radians)
{
    return radians * 180.0f / PI;
}

Scalar det(const vec3& v1, const vec3& v2, const vec3& v3)
{
    return v1[0] * (v2[1] * v3[2] - v2[2] * v3[1]) - v2[0] * (v1[1] * v3[2] - v1[2] * v3[1]) +
           v3[0] * (v1[1] * v2[2] - v1[2] * v2[1]);
}

vec3 pointwise_min(const vec3& a, const vec3& b)
{
    return vec3(std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2]));
}

vec3 pointwise_max(const vec3& a, const vec3& b)
{
    return vec3(std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2]));
}
} // namespace radiant