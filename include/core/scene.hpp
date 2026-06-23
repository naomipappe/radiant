#pragma once

#include "imgui.h"

#include "core/material.hpp"
#include "core/primitive.hpp"
#include "core/mesh.hpp"
#include "core/camera.hpp"
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
        m_dirty = m_dirty || ImGui::InputScalarN("Camera position", ImGuiDataType_Double, m_camera->look_from().m_data, 3, &max);
        m_dirty = m_dirty || ImGui::InputScalarN("Camera target", ImGuiDataType_Double, m_camera->look_at().m_data, 3, &max);
        for (const auto& obj : m_materials)
        {
            m_dirty = m_dirty ||
                      ImGui::SliderScalarN("RGB", ImGuiDataType_Double, &obj->m_emissive.m_data[0], 3, &min, &max, "%.3f");
        }
    }

    bool                                   m_dirty{ false };
    Camera*                                m_camera;
    std::vector<Mesh>                      m_meshes;
    std::vector<std::shared_ptr<Material>> m_materials;
    std::vector<Primitive>                 m_primitives;
};

} // namespace radiant