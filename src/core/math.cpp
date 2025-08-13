#include <ctime>
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

bool test_intersection_aabb(const Ray& r, vec3 aabb_min, vec3 aabb_max)
{
    Scalar t_min = -std::numeric_limits<Scalar>::max();
    Scalar t_max = std::numeric_limits<Scalar>::max();
    for (u32 axis = 0; axis < 3; ++axis)
    {
        const Scalar inv_d    = 1.0 / r.m_direction[axis];
        Scalar       t_axis_1 = (aabb_min[axis] - r.m_origin[axis]) * inv_d;
        Scalar       t_axis_2 = (aabb_max[axis] - r.m_origin[axis]) * inv_d;

        t_min = std::max(t_min, std::min(t_axis_1, t_axis_2));
        t_max = std::min(t_max, std::max(t_axis_1, t_axis_2));
    }
    return t_max >= t_min && t_max > 0;
}

} // namespace radiant