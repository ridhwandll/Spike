/*****************************************************************************/
/*                             Spike SourceCode                              */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: None                                                       */
/*                                                                           */
/*   Licensed under the Apache License, Version 2.0 (the "License");         */
/*   you may not use this file except in compliance with the License.        */
/*   You may obtain a copy of the License at                                 */
/*                                                                           */
/*       http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                           */
/*   Unless required by applicable law or agreed to in writing, software     */
/*   distributed under the License is distributed on an "AS IS" BASIS,       */
/*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*   See the License for the specific language governing permissions and     */
/*   limitations under the License.                                          */
/*****************************************************************************/
#include "SceneHierarchyPanel.h"
#include "../UIUtils/UIUtils.h"
#include "Spike/Scene/Components.h"
#include "Spike/Core/Input.h"
#include <imgui/imgui.h>
#include "../FontAwesome.h"

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
            Entity entity{ entityID, m_Context.get() };
            DrawEntityNode(entity);
        });

        m_IsHierarchyFocused = ImGui::IsWindowFocused();
        m_IsHierarchyHovered = ImGui::IsWindowHovered();

        if (ImGui::IsMouseDown(0) && m_IsHierarchyHovered)
            m_SelectionContext = {};

        //Right Click on a blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                m_SelectionContext = m_Context->CreateEntity("Empty Entity");
            }
            if (ImGui::MenuItem("Create Camera"))
            {
                m_SelectionContext = m_Context->CreateEntity("Camera");
                m_SelectionContext.AddComponent<CameraComponent>();
            }
            if (ImGui::MenuItem("Create Sprite"))
            {
                m_SelectionContext = m_Context->CreateEntity("Sprite");
                m_SelectionContext.AddComponent<SpriteRendererComponent>();
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
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
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