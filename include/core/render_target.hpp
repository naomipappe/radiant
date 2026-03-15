#pragma once

#include "core/types.hpp"
#include "core/vec.hpp"
#include <vector>

namespace radiant
{

struct RenderTarget
{
    std::vector<vec3> render_target;
    std::vector<vec3> accumulator;
    u32               frame{ 0 };
    u32               width{ 0 }, height{ 0 };
};
} // namespace radiant
