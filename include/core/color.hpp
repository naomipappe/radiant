#pragma once

#include <algorithm>
#include <core/vec.hpp>
#include <ostream>

namespace radiant
{
// TODO: Always convert the data before presenting or keep the data in a particular format

struct rgb_color : public vec3
{
    using vec3::vec3;

    rgb_color() : vec3(0.0, 0.0, 0.0) {}
    // This is stupid, but it works
    rgb_color(const vec3& base) : vec3(base) {}

    Scalar r() const { return m_data[0]; }
    Scalar g() const { return m_data[1]; }
    Scalar b() const { return m_data[2]; }
};

inline std::ostream& operator<<(std::ostream& out, const rgb_color& color)
{
    out << color.r() << ' ' << color.g() << ' ' << color.b();
    return out;
}

} // namespace radiant
