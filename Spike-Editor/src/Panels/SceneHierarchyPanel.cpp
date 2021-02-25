/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : SceneHierarchyPanel
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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "SceneHierarchyPanel.h"
#include "Spike/Renderer/RendererAPISwitch.h"
#include "UIUtils/UIUtils.h"
#include "Spike/Scene/Components.h"
#include "Spike/Core/Input.h"
#include "Spike/Utility/FileDialogs.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <FontAwesome.h>
#include <stb_image/stb_image.h>

namespace Spike
{
    template<typename T, typename UIFunction>
    static void DrawComponent(const String& name, Entity entity, UIFunction uiFunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.HasComponent<T>())
        {
            ImGui::PushID((void*)typeid(T).hash_code());
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();

            bool open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button(ICON_FK_PLUS_CIRCLE, ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("Component Settings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("Component Settings"))
            {
                if (ImGui::MenuItem("Reset"))
                    component.Reset();
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open)
            {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<T>();

            ImGui::PopID();
        }
    }

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
        m_SelectionContext = {};

        if (m_SelectionContext && false)
        {
            // Try to find same entity in new scene
            auto& entityMap = m_Context->GetEntityMap();
            UUID selectedEntityID = m_SelectionContext.GetUUID();
            if (entityMap.find(selectedEntityID) != entityMap.end())
                m_SelectionContext = entityMap.at(selectedEntityID);
        }
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        // Hierarchy
        ImGui::Begin("Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity{ entityID, m_Context.Raw() };
            if (entity.HasComponent<IDComponent>())
                DrawEntityNode(entity);
        });

        m_IsHierarchyFocused = ImGui::IsWindowFocused();
        m_IsHierarchyHovered = ImGui::IsWindowHovered();

        if (ImGui::IsMouseDown(0) && m_IsHierarchyHovered)
            m_SelectionContext = {};

        //Right Click on a blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Empty Entity"))
            {
                m_SelectionContext = m_Context->CreateEntity("Entity");
            }
            if (ImGui::MenuItem("Camera"))
            {
                m_SelectionContext = m_Context->CreateEntity("Camera");
                m_SelectionContext.AddComponent<CameraComponent>();
            }
            if (ImGui::MenuItem("Sprite"))
            {
                m_SelectionContext = m_Context->CreateEntity("Sprite");
                m_SelectionContext.AddComponent<SpriteRendererComponent>();
            }
            if (ImGui::MenuItem("Mesh"))
            {
                m_SelectionContext = m_Context->CreateEntity("Mesh");
                m_SelectionContext.AddComponent<MeshComponent>();
            }
            ImGui::EndPopup();
        }
        ImGui::End();


        // Inspector
        ImGui::Begin(ICON_FK_INFO_CIRCLE" Inspector");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {

        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;

            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::TreePop();
        }

        if (entityDeleted)
        {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
        }

    }

    void SceneHierarchyPanel::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SPK_BIND_EVENT_FN(SceneHierarchyPanel::OnKeyPressed));
    }

    bool SceneHierarchyPanel::OnKeyPressed(KeyPressedEvent& e)
    {
        if (Input::IsKeyPressed(Key::Delete) && m_IsHierarchyFocused)
        {
            if (m_SelectionContext)
                m_Context->DestroyEntity(m_SelectionContext);
            m_SelectionContext = {};
            return false;
        }
        if ((Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl)) && Input::IsKeyPressed(Key::D))
            m_Context->DuplicateEntity(m_SelectionContext);

        return false;
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        auto ID = entity.GetComponent<IDComponent>().ID;

        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = String(buffer);
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
                    SPK_CORE_LOG_WARN("This entity already has Transform component!");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Camera"))
            {
                if (!entity.HasComponent<CameraComponent>())
                    entity.AddComponent<CameraComponent>();
                else
                    SPK_CORE_LOG_WARN("This entity already has Camera component!");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                if (!entity.HasComponent<SpriteRendererComponent>())
                    entity.AddComponent<SpriteRendererComponent>();
                else
                    SPK_CORE_LOG_WARN("This entity already has Sprite Renderer component!");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Mesh"))
            {
                if (!entity.HasComponent<MeshComponent>())
                    entity.AddComponent<MeshComponent>();
                else
                    SPK_CORE_LOG_WARN("This entity already has Mesh component!");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("RigidBody2D"))
            {
                if (!entity.HasComponent<RigidBody2DComponent>())
                    entity.AddComponent<RigidBody2DComponent>();
                else
                    SPK_CORE_LOG_WARN("This entity already has RigidBody2D component!");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("BoxCollider2D"))
            {
                if (!entity.HasComponent<BoxCollider2DComponent>())
                    entity.AddComponent<BoxCollider2DComponent>();
                else
                    SPK_CORE_LOG_WARN("This entity already has BoxCollider2D component!");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("CircleCollider2D"))
            {
                if (!entity.HasComponent<CircleCollider2DComponent>())
                    entity.AddComponent<CircleCollider2DComponent>();
                else
                    SPK_CORE_LOG_WARN("This entity already has CircleCollider2D component!");
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>(ICON_FK_WRENCH" Transform", entity, [](auto& component)
        {
            GUI::DrawVec3Control("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            GUI::DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            GUI::DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<CameraComponent>(ICON_FK_CAMERA" Camera", entity, [](auto& component)
        {
            auto& camera = component.Camera;
            GUI::DrawBoolControl("Primary", &component.Primary, 160.0f);

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
            ImGui::Columns(2);
            ImGui::Text("Projection");
            ImGui::SetColumnWidth(0, 160.0f);
            ImGui::NextColumn();
            if (ImGui::BeginCombo("##Projection", currentProjectionTypeString))
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
            ImGui::Columns(1);
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
                if (GUI::DrawFloatControl("Vertical FOV", &verticalFOV, 160.0f))
                    camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

                float nearClip = camera.GetPerspectiveNearClip();
                if (GUI::DrawFloatControl("Near Clip", &nearClip, 160.0f))
                    camera.SetPerspectiveNearClip(nearClip);

                float farClip = camera.GetPerspectiveFarClip();
                if (GUI::DrawFloatControl("Far Clip", &farClip, 160.0f))
                    camera.SetPerspectiveFarClip(farClip);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                float orthoSize = camera.GetOrthographicSize();
                if (GUI::DrawFloatControl("Size", &orthoSize, 160.0f))
                    camera.SetOrthographicSize(orthoSize);

                float nearClip = camera.GetOrthographicNearClip();
                if (GUI::DrawFloatControl("Near Clip", &nearClip, 160.0f))
                    camera.SetOrthographicNearClip(nearClip);

                float farClip = camera.GetOrthographicFarClip();
                if (GUI::DrawFloatControl("Far Clip", &farClip, 160.0f))
                    camera.SetOrthographicFarClip(farClip);

                GUI::DrawBoolControl("Fixed Aspect Ratio", &component.FixedAspectRatio, 160.0f);
            }
        });

        DrawComponent<SpriteRendererComponent>(ICON_FK_SQUARE_O" Sprite Renderer", entity, [](auto& component)
        {
            GUI::DrawColorControl("Color", component.Color);

            const RendererID imageID = component.Texture.Raw() == nullptr ? 0 : component.Texture->GetRendererID();

            #ifdef RENDERER_API_OPENGL
                stbi_set_flip_vertically_on_load(true);
            #endif

            ImGui::Text("Texture");
            const float cursorPos = ImGui::GetCursorPosY();
            ImGui::SameLine(ImGui::GetWindowWidth() * 0.8f);

            if(GUI::DrawImageButtonControl(imageID, { 65, 65 }))
            {
                char const* lFilterPatterns[8] = { "*.png", "*.jpg", "*.tga", "*.bmp", "*.psd", "*.hdr", "*.pic", "*.gif" };
                const char* filepath = FileDialogs::OpenFile("Open Texture", "", 8, lFilterPatterns, "", false);
                if (filepath)
                    component.SetTexture(filepath);
            }

            ImGui::SetCursorPosY(cursorPos + 5);

            if (ImGui::Button("Open Texture"))
            {
                char const* lFilterPatterns[8] = { "*.png", "*.jpg", "*.tga", "*.bmp", "*.psd", "*.hdr", "*.pic", "*.gif" };
                const char* filepath = FileDialogs::OpenFile("Open Texture", "", 8, lFilterPatterns, "", false);
                if (filepath)
                    component.SetTexture(filepath);
            }

            ImGui::SameLine();

            if (ImGui::Button("Remove Texture"))
                component.RemoveTexture();

            // Tiling Factor
            GUI::DrawFloatControl("Tiling Factor", &component.TilingFactor, 100);
        });

        DrawComponent<MeshComponent>(ICON_FK_CUBE" Mesh", entity, [](auto& component)
        {
            ImGui::Text("File Path");
            ImGui::SameLine();
            if (component.Mesh)
                ImGui::InputText("##meshfilepath", (char*)component.Mesh->m_FilePath.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
            else
                ImGui::InputText("##meshfilepath", (char*)"", 256, ImGuiInputTextFlags_ReadOnly);

            if (ImGui::Button("Open"))
            {
                const char* patterns[2] = { "*.fbx", "*.obj" };
                const char* file = FileDialogs::OpenFile("Open 3D Object file", "", 2, patterns, "", false);
                if (file)
                {
                    component.Mesh = Ref<Mesh>::Create(file);
                    component.SetFilePath(String(file));
                }
            }
            if (component.Mesh)
            {
                ImGui::SameLine();
                bool remove = false;
                if (ImGui::Button("Remove"))
                {
                    component.Reset();
                    remove = true;
                }
                ImGui::SameLine();
                if (!remove)
                {
                    if (ImGui::Button("Reload"))
                        component.Mesh->Reload();
                }
            }
        });

        DrawComponent<RigidBody2DComponent>("RigidBody2D", entity, [](auto& component)
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
                GUI::DrawFloatControl("Gravity Scale", &component.Gravity, 160.0f);
                GUI::DrawBoolControl("Fixed Rotation", &component.FixedRotation, 160.0f);
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
        DrawComponent<BoxCollider2DComponent>("BoxCollider2D", entity, [](auto& component)
        {
            GUI::DrawFloat2Control("Offset",   component.Offset, 150.0f);
            GUI::DrawFloat2Control("Size",     component.Size, 150.0f);
            GUI::DrawFloatControl("Density",  &component.Density, 150.0f);
            GUI::DrawFloatControl("Friction", &component.Friction, 150.0f);
            GUI::DrawBoolControl("Show Collider Bounds", &component.ShowBounds, 150.0f);
        });
        DrawComponent<CircleCollider2DComponent>("CircleCollider2D", entity, [](auto& component)
        {
            GUI::DrawFloat2Control("Offset",   component.Offset);
            GUI::DrawFloatControl("Radius",   &component.Radius);
            GUI::DrawFloatControl("Density",  &component.Density);
            GUI::DrawFloatControl("Friction", &component.Friction);
        });
    }
}