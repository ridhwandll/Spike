//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Log.h"
#include "Spike/Core/Base.h"
#include "Spike/Scene/Entity.h"
#include "Spike/Events/Event.h"
#include "Spike/Events/KeyEvent.h"

namespace Spike
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);
        void SetContext(const Ref<Scene>& context);
        void ClearSelectedEntity() { m_SelectionContext = {}; }

        Entity GetSelectedEntity() const { return m_SelectionContext; };
        void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }
        Ref<Scene> GetCurrentScene() const { return m_Context; };

        void OnImGuiRender();
        void OnEvent(Event& e);
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

        Ref<Scene> m_Context;
    private:
        bool m_IsHierarchyFocused = false;
        bool m_IsHierarchyHovered = false;
        Entity m_SelectionContext;
    };
}