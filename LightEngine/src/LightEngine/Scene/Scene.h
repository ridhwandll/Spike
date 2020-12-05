#pragma once

#pragma warning(push, 0)
#include <entt.hpp>
#pragma warning(pop)

#include "LightEngine/Core/Timestep.h"


namespace LightEngine
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        entt::registry m_Registry;
        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}