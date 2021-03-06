/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : MaterialPanel
File Type      : cpp
File created on: 2021/03/06
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "MaterialPanel.h"
#include "UIUtils/UIUtils.h"
#include "Spike/Utility/FileDialogs.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Spike
{
    void MaterialPanel::OnImGuiRender(Entity& selectedEntity)
    {
        ImGui::Begin("Material Inspector");

        if (selectedEntity && selectedEntity.HasComponent<MeshComponent>())
        {
            auto& mesh = selectedEntity.GetComponent<MeshComponent>().Mesh;
            if (mesh)
            {
                auto material = mesh->GetMaterial();
                ImGui::TextColored(ImVec4(0.1f, 0.9f, 0.1f, 1.0f), "Shader: %s", material->m_Shader->GetName().c_str());
                ImGui::Separator();
                GUI::DrawColorControl3("Color", material->m_Color, 150.0f);
                GUI::DrawBoolControl("Use Diffuse Texture(s)", &material->m_AlbedoTexToggle, 150.0f);
            }
        }
        ImGui::End();
    }
}
