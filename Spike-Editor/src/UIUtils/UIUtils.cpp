/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : UIUtils
File Type      : cpp
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
#include "UIUtils.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <FontAwesome.h>

namespace Spike::GUI
{
    bool DrawScriptTextControl(const char* label, std::string& value, float columnWidth, bool foundScript)
    {
        bool modified = false;
        ImGui::PushID(label);

        if (!foundScript && value != "SpikeNull")
            ImGui::TextColored({ 0.9, 0.1f, 0.1f, 1.0f }, ICON_FK_TIMES" Not Connected with ScriptEngine");
        if (value == "SpikeNull")
            ImGui::TextColored({ 1.0, 1.0f, 0.0f, 1.0f }, "SpikeNull is used");
        if (foundScript && value != "SpikeNull")
            ImGui::TextColored({ 0.1f, 0.9f, 0.1f, 1.0f }, "Connected with ScriptEngine");

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (!foundScript && value != "SpikeNull")
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.1f, 0.1f, 1.0f });
        if (value == "SpikeNull")
            ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 0.0f, 1.0f });
        if (foundScript && value != "SpikeNull")
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f, 0.9f, 0.1f, 1.0f });

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), value.c_str());

        if (ImGui::InputText("##value", buffer, sizeof(buffer)))
        {
            value = buffer;
            modified = true;
        }

        ImGui::PopStyleColor();
        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool DrawTextControl(const char* label, char* value, float columnWidth)
    {
        bool modified = false;

        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), value);

        if (ImGui::InputText("##value", buffer, sizeof(buffer)))
        {
            value = buffer;
            modified = true;
        }

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawBoolControl(const char* label, bool* boolean, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (ImGui::Checkbox("##value", boolean))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawIntControl(const char* label, int* value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (ImGui::DragInt("##value", value, 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloatControl(const char* label, float* value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (ImGui::DragFloat("##value", value, 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloat2Control(const char* label, glm::vec2& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (ImGui::DragFloat2("##value", glm::value_ptr(value), 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloat3Control(const char* label, glm::vec3& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (ImGui::DragFloat3("##value", glm::value_ptr(value), 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloat4Control(const char* label, glm::vec4& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (ImGui::DragFloat4("##value", glm::value_ptr(value), 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawColorControl(const char* label, glm::vec4& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        ImGui::PushItemWidth(-std::numeric_limits<float>::min());
        if (ImGui::ColorEdit4("##value", glm::value_ptr(value)))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar(2);

        ImGui::Columns(1);

        ImGui::PopID();
    }
}
