#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace LightEngine
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            LE_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            LE_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            LE_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        void RemoveAllComponent()
        {
            m_Scene->m_Registry.remove_all(m_EntityHandle);
        }

        template<typename T>
        void AddNativeScript()
        {
            AddComponent<NativeScriptComponent>().Bind<T>();
        }

        operator bool() const { return m_EntityHandle != entt::null; }
    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene = nullptr;
    };
}