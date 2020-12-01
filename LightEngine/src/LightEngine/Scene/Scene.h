#pragma once
#include <entt.hpp>
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
        void OnUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);
    private:
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        entt::registry m_Registry;
        friend class Entity;
    };
}