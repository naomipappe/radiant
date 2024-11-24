#pragma once

#include <algorithm>
#include <core/vec.hpp>
#include <fstream>

namespace radiant
{

using rgb_color = vec3f;

inline std::ostream& operator<<(std::ostream& out, const rgb_color& color)
{
    for (u32 i = 0; i < 3; ++i)
    {
        out << std::clamp(static_cast<u32>(color[i] * 255), (u32)0, (u32)255) << ' ';
    }
    return out;
}

} // namespace radiant
