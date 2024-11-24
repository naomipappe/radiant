#pragma once

#include <algorithm>
#include <common/vec.hpp>

namespace radiant
{

struct rgb_color : public vec3ui
{
    rgb_color(const vec3f& v)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            m_data[i] = static_cast<u32>(std::clamp(255.0f * v[i], 0.0f, 255.0f));
        }
    }

    const u32 r() const { return m_data[0]; }
    const u32 g() const { return m_data[1]; }
    const u32 b() const { return m_data[2]; }
};

struct hsv_color : public vec3ui
{
    hsv_color(const vec3f& v)
    {
        for (u32 i = 0; i < 3; ++i)
        {
            m_data[i] = static_cast<u32>(std::clamp(255.0f * v[i], 0.0f, 255.0f));
        }
    }

    const u32 h() const { return m_data[0]; }
    const u32 s() const { return m_data[1]; }
    const u32 v() const { return m_data[2]; }
};

} // namespace radiant
