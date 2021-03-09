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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/

//UI utility function to be used in the Panels and the GUI stuff
#pragma once
#include <string>
#include "Spike/Scene/Entity.h"
#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace Spike::GUI
{
    void BeginViewport(const char* name);
    void EndViewport();
    void BeginDockspace();
    void EndDockspace();

    void DrawImageControl(const RendererID imageID, const glm::vec2& viewportDimensions);
    bool DrawImageButtonControl(const RendererID imageID, glm::vec2 buttonSize);
    bool DrawScriptTextControl(const char* label, String& value, float columnWidth = 100.0f, bool foundTheScript = false);
    bool DrawTextControl(const char* label, const char* value, float columnWidth = 100.0f);
    bool DrawTextControlWithoutLabel(String* source);
    void DrawVec3Control(const String& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    bool DrawBoolControl(const char* label, bool* boolean, float columnWidth = 100.0f);
    bool DrawIntControl(const char* label, int* value, float columnWidth = 100.0f);
    bool DrawFloatControl(const char* label, float* value, float columnWidth = 100.0f);
    bool DrawFloat2Control(const char* label, glm::vec2& value, float columnWidth = 100.0f);
    bool DrawFloat3Control(const char* label, glm::vec3& value, float columnWidth = 100.0f);
    bool DrawFloat4Control(const char* label, glm::vec4& value, float columnWidth = 100.0f);
    bool DrawColorControl4(const char* label, glm::vec4& value, float columnWidth = 100.0f);
    bool DrawColorControl3(const char* label, glm::vec3& value, float columnWidth = 100.0f);
    void DrawToggleButton(const char* text, const ImVec4& color, bool* boolToToggle);

    /* [Spike] A dynamic toggle button is a ImGui::Button(...) wrapper that draws label changing button.
     * This is basically a boolean control. This was made because in ImGui::Checkbox(...) a label is necssary
     * but in this button, no label is necessary. Making it perfect for oneliners. [Spike] */
    void DrawDynamicToggleButton(const char* offLabel, const char* onLabel, const ImVec4& offColor, const ImVec4& onColor, bool* boolToModify);
    void DrawColorChangingToggleButton(const char* label, const ImVec4& offBgColor, const ImVec4& onBgColor, const ImVec4& textColor, bool* boolToModify);

    template<typename OnFunction, typename OffFunction>
    void DrawDynamicSwitch(const char* offLabel, const char* onLabel, OffFunction offFunc, OnFunction onFunc, bool* boolToModify)
    {
        if (*boolToModify)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.5f, 0.2f, 1.0f });
            if (ImGui::Button(onLabel))
            {
                offFunc();
                *boolToModify = false;
            }
        }
        else if (!*boolToModify)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.7f, 0.1f, 0.1f, 1.0f });
            if (ImGui::Button(offLabel))
            {
                onFunc();
                *boolToModify = true;
            }
        }
        ImGui::PopStyleColor();
    }
}

