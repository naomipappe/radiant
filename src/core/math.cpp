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
} // namespace radiant