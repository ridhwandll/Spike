#pragma once
#include "entt.h"
#include "LightEngine/Core/Timestep.h"

namespace LightEngine
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();
        entt::registry& GetReg() { return m_Registry; }

        void OnUpdate(Timestep ts);
    private:
        entt::registry m_Registry;
    };
}