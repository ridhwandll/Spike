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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "SceneHierarchyPanel.h"
#include "../UIUtils/UIUtils.h"
#include "Spike/Scene/Components.h"
#include "Spike/Core/Input.h"
#include "Spike/Renderer/MeshGenerator.h"
#include <imgui/imgui.h>
#include <FontAwesome.h>

namespace Spike
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        // Hierarchy
        ImGui::Begin("Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity{ entityID, m_Context.Raw() };
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
            if (ImGui::MenuItem("Cube"))
            {
                m_SelectionContext = m_Context->CreateEntity("Cube");
                m_SelectionContext.AddComponent<MeshComponent>().Mesh = MeshGenerator::CreateCube({ 1.0f, 1.0f, 1.0f });
            }
            if (ImGui::MenuItem("Sphere"))
            {
                m_SelectionContext = m_Context->CreateEntity("Sphere");
                m_SelectionContext.AddComponent<MeshComponent>().Mesh = MeshGenerator::CreateSphere(1.0f);
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
        return false;
    }
}