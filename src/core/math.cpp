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
    return v1[0] * v2[1] * v3[2] + v1[1] * v2[0] * v3[2] + v3[1] * v2[0] * v1[2] - v1[2] * v2[1] * v3[0] -
           v1[1] * v2[0] * v3[2] - v2[2] * v3[1] * v1[0];
}
} // namespace radiant