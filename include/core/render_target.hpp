#pragma once

#include "core/color.hpp"
#include "core/types.hpp"
#include <vector>
namespace radiant
{
struct RenderTarget
{
    std::vector<rgba> render_target;
    u32                    width{ 0 }, height{ 0 };
};
} // namespace radiant
