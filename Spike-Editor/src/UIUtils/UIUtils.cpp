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
#include "Spike/Scene/Components.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Spike/Utility/PlatformUtils.h"
#include "Spike/Physics/2D/Physics2D.h"
#include "Panels/ConsolePanel.h"
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <string>

namespace Spike
{
    void DrawBoolControl(const char* label, bool* boolean, float columnWidth)
    {
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::Checkbox("##value", boolean);

        ImGui::Columns(1);
        ImGui::PopID();
    }

    static void DrawFloatControl(const char* label, float* value, float columnWidth)
    {
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::DragFloat("##value", value, 0.1f);

        ImGui::Columns(1);
        ImGui::PopID();
    }

    void DrawFloat2Control(const char* label, glm::vec2& value, float columnWidth)
    {
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::DragFloat2("##value", glm::value_ptr(value), 0.1f);

        ImGui::Columns(1);
        ImGui::PopID();
    }

    void DrawColorControl(const char* label, glm::vec4& value, float columnWidth)
    {
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-std::numeric_limits<float>::min());
        ImGui::ColorEdit4("##value", glm::value_ptr(value));

        ImGui::Columns(1);
        ImGui::PopID();
    }

    template<typename ComponentType, typename UIFunction>
    static void DrawComponent(Entity entity, UIFunction uiFunction)
    {
        if (entity.HasComponent<ComponentType>())
        {
            const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            auto& component = entity.GetComponent<ComponentType>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 1.0f, 1.0f });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

            bool open = ImGui::TreeNodeEx((void*)typeid(ComponentType).hash_code(), treeNodeFlags, component.GetUITitle());
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

            if (ImGui::Button(ICON_FK_PLUS_CIRCLE, ImVec2{ lineHeight, lineHeight }))
                ImGui::OpenPopup("Component Settings");

            ImGui::PopStyleVar();

            bool removeComponent = false;
            if (ImGui::BeginPopup("Component Settings"))
            {
                if (ImGui::MenuItem("Reset"))
                    component.Reset();
                if (ImGui::MenuItem("Remove Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open)
            {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<ComponentType>();
        }
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

    void DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("Add Component");

        ImGui::TextDisabled("UUID: %llx", entity.GetComponent<IDComponent>().ID);
        if (ImGui::BeginPopup("Add Component"))
        {
            if (ImGui::MenuItem("Transform"))
            {
                if (!entity.HasComponent<TransformComponent>())
                    entity.AddComponent<TransformComponent>();
                else
                    Console::Get()->Print("This entity already has Transform component!", Console::LogLevel::LVL_WARN);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Camera"))
            {
                if (!entity.HasComponent<CameraComponent>())
                    entity.AddComponent<CameraComponent>();
                else
                    Console::Get()->Print("This entity already has Camera component!", Console::LogLevel::LVL_WARN);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                if (!entity.HasComponent<SpriteRendererComponent>())
                    entity.AddComponent<SpriteRendererComponent>();
                else
                    Console::Get()->Print("This entity already has Sprite Renderer component!", Console::LogLevel::LVL_WARN);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Mesh"))
            {
                if (!entity.HasComponent<MeshComponent>())
                    entity.AddComponent<MeshComponent>();
                else
                    Console::Get()->Print("This entity already has Mesh component!", Console::LogLevel::LVL_WARN);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("RigidBody2D"))
            {
                if (!entity.HasComponent<RigidBody2DComponent>())
                    entity.AddComponent<RigidBody2DComponent>();
                else
                    Console::Get()->Print("This entity already has RigidBody2D component!", Console::LogLevel::LVL_WARN);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("BoxCollider2D"))
            {
                if (!entity.HasComponent<BoxCollider2DComponent>())
                    entity.AddComponent<BoxCollider2DComponent>();
                else
                    Console::Get()->Print("This entity already has BoxCollider2D component!", Console::LogLevel::LVL_WARN);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("CircleCollider2D"))
            {
                if (!entity.HasComponent<CircleCollider2DComponent>())
                    entity.AddComponent<CircleCollider2DComponent>();
                else
                    Console::Get()->Print("This entity already has CircleCollider2D component!", Console::LogLevel::LVL_WARN);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>(entity, [](auto& component)
        {
            DrawVec3Control("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<CameraComponent>(entity, [](auto& component)
        {
            auto& camera = component.Camera;

            ImGui::Checkbox("Primary", &component.Primary);

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                    {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
                if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
                    camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

                float nearClip = camera.GetPerspectiveNearClip();
                if (ImGui::DragFloat("Near Clip", &nearClip))
                    camera.SetPerspectiveNearClip(nearClip);

                float farClip = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Far Clip", &farClip))
                    camera.SetPerspectiveFarClip(farClip);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                float orthoSize = camera.GetOrthographicSize();
                if (ImGui::DragFloat("Size", &orthoSize))
                    camera.SetOrthographicSize(orthoSize);

                float nearClip = camera.GetOrthographicNearClip();
                if (ImGui::DragFloat("Near Clip", &nearClip))
                    camera.SetOrthographicNearClip(nearClip);

                float farClip = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Far Clip", &farClip))
                    camera.SetOrthographicFarClip(farClip);

                ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
            }
        });

        DrawComponent<SpriteRendererComponent>(entity, [](auto& component)
        {
            DrawColorControl("Color", component.Color);

            const uint64_t id = component.Texture.Raw() == nullptr ? 0 : component.Texture->GetRendererID();
            stbi_set_flip_vertically_on_load(1);

            ImGui::Text("Texture");
            const float cursorPos = ImGui::GetCursorPosY();
            const ImVec2 buttonSize = { 65, 65 };
            ImGui::SameLine(ImGui::GetWindowWidth() * 0.8f);

            if (ImGui::ImageButton((ImTextureID)id, buttonSize, { 0, 1 }, { 1, 0 }, 0, {1, 0, 1, 1}))
            {
                std::string filepath = FileDialogs::OpenFile("Texture (*.png)(*.jpg)*.png**.jpg*\0");
                if (!filepath.empty())
                    component.SetTexture(filepath);
            }

            ImGui::SetCursorPosY(cursorPos + 5);

            if (ImGui::Button("Add Texture"))
            {
                std::string filepath = FileDialogs::OpenFile("Texture (*.png)(*.jpg)*.png**.jpg*\0");
                if (!filepath.empty())
                    component.SetTexture(filepath);
            }

            ImGui::SameLine();

            if (ImGui::Button("Remove Texture"))
                component.RemoveTexture();

            // Tiling Factor
            DrawFloatControl("Tiling Factor", &component.TilingFactor, 120);
        });

        DrawComponent<MeshComponent>(entity, [](auto& component)
        {
            ImGui::Text("File Path");
            ImGui::SameLine();
            if (ImGui::Button("Open Mesh", ImVec2(100, 20)))
            {
                std::string file = FileDialogs::OpenFile("ObjectFile (*.fbx *.obj *.blend)\0*.fbx; *.obj; *.blend\0");
                if (!file.empty())
                {
                    component.Mesh = Ref<Mesh>::Create(file);
                    component.SetFilePath(file);
                }
            }
            ImGui::SameLine();

            if (ImGui::Button("Remove Mesh", ImVec2(100, 20)))
            {
                if (component.Mesh)
                    component.Reset();
            }

            ImGui::PushItemWidth(ImGui::GetWindowWidth() - 50);

            if (component.Mesh)
                ImGui::InputText("##meshfilepath", (char*)component.Mesh->m_FilePath.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
            else
                ImGui::InputText("##meshfilepath", (char*)"", 256, ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
        });

        DrawComponent<RigidBody2DComponent>(entity, [](auto& component)
        {
            const char* rb2dTypeStrings[3] = { "Static", "Dynamic", "Kinematic" };
            const char* currentType = rb2dTypeStrings[(int)component.BodyType];
            {
                ImGui::Columns(2);
                ImGui::Text("Type");
                ImGui::SetColumnWidth(0, 160.0f);
                ImGui::NextColumn();
                if (ImGui::BeginCombo("##type", currentType))
                {
                    for (int type = 0; type < 3; type++)
                    {
                        bool is_selected = (currentType == rb2dTypeStrings[type]);
                        if (ImGui::Selectable(rb2dTypeStrings[type], is_selected))
                        {
                            currentType = rb2dTypeStrings[type];
                            component.BodyType = (RigidBody2DComponent::Type)type;
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::Columns(1);
            }

            if (component.BodyType == RigidBody2DComponent::Type::Dynamic)
            {
                DrawFloatControl("Gravity Scale", &component.Gravity, 160.0f);
                DrawBoolControl("Fixed Rotation", &component.FixedRotation, 160.0f);
            }

            {
                if (component.BodyType == RigidBody2DComponent::Type::Dynamic || component.BodyType == RigidBody2DComponent::Type::Kinematic)
                {
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 160.0f);
                    ImGui::Text("Collision Detection");
                    ImGui::NextColumn();
                    const char* rb2dCollisionTypeStrings[2] = { "Discrete", "Continuous" };
                    const char* current_item = rb2dCollisionTypeStrings[(int)component.CollisionDetection];
                    if (ImGui::BeginCombo("##collisiondetection", current_item))
                    {
                        for (int type = 0; type < 2; type++)
                        {
                            bool is_selected = (current_item == rb2dCollisionTypeStrings[type]);
                            if (ImGui::Selectable(rb2dCollisionTypeStrings[type], is_selected))
                            {
                                current_item = rb2dCollisionTypeStrings[type];
                                component.CollisionDetection = (CollisionDetectionType)type;
                            }

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::Columns(1);
                    {
                        ImGui::Columns(2);
                        ImGui::SetColumnWidth(0, 160.0f);
                        ImGui::Text("Sleep Type");
                        ImGui::NextColumn();
                        const char* rb2dSleepTypeStrings[3] = { "NeverSleep", "StartAwake", "StartAsleep" };
                        const char* current_item = rb2dSleepTypeStrings[(int)component.Sleeptype];
                        if (ImGui::BeginCombo("##sleeptype", current_item))
                        {
                            for (int type = 0; type < 3; type++)
                            {
                                bool is_selected = (current_item == rb2dSleepTypeStrings[type]);
                                if (ImGui::Selectable(rb2dSleepTypeStrings[type], is_selected))
                                {
                                    current_item = rb2dSleepTypeStrings[type];
                                    component.Sleeptype = (SleepType)type;
                                }

                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::Columns(1);
                    }
                }
            }

        });
        DrawComponent<BoxCollider2DComponent>(entity, [](auto& component)
        {
            DrawFloat2Control("Offset",   component.Offset);
            DrawFloat2Control("Size",     component.Size);
            DrawFloatControl("Density",  &component.Density);
            DrawFloatControl("Friction", &component.Friction);
        });
        DrawComponent<CircleCollider2DComponent>(entity, [](auto& component)
        {
            DrawFloat2Control("Offset",   component.Offset);
            DrawFloatControl("Radius",   &component.Radius);
            DrawFloatControl("Density",  &component.Density);
            DrawFloatControl("Friction", &component.Friction);
        });
    }
}
