#pragma once
#include "LightEngine/Scene/Scene.h"
#include "LightEngine/Core/Log.h"
#include "LightEngine/Core/Base.h"
#include "LightEngine/Scene/Entity.h"

namespace LightEngine
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);
        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
    private:
        Ref<Scene> m_Context;
        Entity m_SelctionContext;
    };
}