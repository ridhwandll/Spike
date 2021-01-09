/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : UIUtils
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/

//UI utility function to be used in the Panels and the GUI stuff
#pragma once
#include <string>
#include "Spike/Scene/Entity.h"
#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace Spike
{
    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    void DrawComponents(Entity entity);
    void DrawFloatControl(const char* label, float* value, float columnWidth = 100.0f);
    void DrawColorControl(const char* label, glm::vec4& value, float columnWidth = 100.0f);

    template<typename UIFunction>
    void DrawToggleButton(const char* text, ImVec4 color, UIFunction uiFunction)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        if (ImGui::Button(text))
        {
            uiFunction();
        }
        ImGui::PopStyleColor();
    }
}