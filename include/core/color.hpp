#pragma once

#include <algorithm>
#include <core/vec.hpp>
#include <fstream>

namespace radiant
{
// TODO: Always convert the data before presenting or keep the data in a particular format

struct rgb_color : public vec3f
{
    using vec3f::vec3f;

    // This is stupid, but it works
    rgb_color(const vec3f& base) : vec3f(base) {}

    u8 r() const { return std::clamp(static_cast<u32>(m_data[0] * 255), static_cast<u32>(0), static_cast<u32>(255)); }
    u8 g() const { return std::clamp(static_cast<u32>(m_data[1] * 255), static_cast<u32>(0), static_cast<u32>(255)); }
    u8 b() const { return std::clamp(static_cast<u32>(m_data[2] * 255), static_cast<u32>(0), static_cast<u32>(255)); }
};

inline std::ostream& operator<<(std::ostream& out, const rgb_color& color)
{
    for (u32 i = 0; i < 3; ++i)
    {
        out << std::clamp(static_cast<u32>(color[i] * 255), static_cast<u32>(0), static_cast<u32>(255)) << ' ';
    }
    return out;
}

} // namespace radiant
