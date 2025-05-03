#include <core/math.hpp>

namespace radiant
{
f32 deg_to_rad(f32 degrees)
{
    return degrees * PI / 180.0f;
}
f32 rad_to_get(f32 radians)
{
    return radians * 180.0f / PI;
}
} // namespace radiant