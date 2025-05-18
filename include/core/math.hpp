#pragma once

#include <limits>
#include <cmath>
#include "core/types.hpp"

namespace radiant
{
const Scalar inf  = std::numeric_limits<Scalar>::infinity();
const Scalar PI   = 3.1415926535897932385;
const Scalar PI_2 = PI / 2;
const Scalar PI_3 = PI / 3;
const Scalar PI_4 = PI / 4;
const Scalar PI_6 = PI / 6;

Scalar deg_to_rad(Scalar degrees);
Scalar rad_to_get(Scalar radians);
} // namespace radiant