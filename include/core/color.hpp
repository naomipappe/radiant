#pragma once

#include <core/vec.hpp>
#include <ostream>

namespace radiant
{

struct rgba : public vec4
{
    using vec4::vec4;

    rgba() : vec4(0.0) {}
    // This is stupid, but it works
    rgba(const vec4& base) : vec4(base) {}

    Scalar r() const { return x(); }
    Scalar g() const { return y(); }
    Scalar b() const { return z(); }
    Scalar a() const { return w(); }
};

inline std::ostream& operator<<(std::ostream& out, const rgba& color)
{
    out << color.r() << ' ' << color.g() << ' ' << color.b();
    return out;
}

} // namespace radiant
