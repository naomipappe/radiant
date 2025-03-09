#pragma once

#include <algorithm>
#include <core/vec.hpp>
#include <ostream>

namespace radiant
{
// TODO: Always convert the data before presenting or keep the data in a particular format

struct rgb_color : public vec3f
{
    using vec3f::vec3f;

    rgb_color() : vec3f(0.0f, 0.0f, 0.0f) {}
    // This is stupid, but it works
    rgb_color(const vec3f& base) : vec3f(base) {}

    u8 r() const { return std::clamp(m_data[0], 0.0f, 0.999f) * 256; }
    u8 g() const { return std::clamp(m_data[1], 0.0f, 0.999f) * 256; }
    u8 b() const { return std::clamp(m_data[2], 0.0f, 0.999f) * 256; }
};

inline std::ostream& operator<<(std::ostream& out, const rgb_color& color)
{
    out << color.r() << ' ' << color.g() << ' ' << color.b();
    return out;
}

} // namespace radiant
