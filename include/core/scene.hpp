#pragma once

#include "imgui.h"

#include "core/material.hpp"
#include "core/primitive.hpp"

#include <memory>
#include <vector>

namespace radiant
{

struct Scene
{
    void imgui_traverse()
    {
        double min = 0.0f;
        double max = 1.0f;
        for (const auto& obj : m_materials)
        {
            m_dirty = m_dirty ||  ImGui::SliderScalarN("RGB", ImGuiDataType_Double, &obj->m_emissive.m_data[0], 3,&min, &max, "%.3f");
        }
    }
    bool                                    m_dirty{ false };
    std::vector<std::shared_ptr<Mesh>>      m_meshes;
    std::vector<std::shared_ptr<Material>>  m_materials;
    std::vector<std::shared_ptr<Primitive>> m_primitives;
};

} // namespace radiant