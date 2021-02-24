/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Scene
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Core/Ref.h"
#include "Spike/Core/UUID.h"
#include "Spike/Renderer/EditorCamera.h"
#include "Spike/Core/Timestep.h"
#include "Spike/Renderer/Framebuffer.h"

#pragma warning(push, 0)
#include <entt.hpp>
#pragma warning(pop)

namespace Spike
{
    class Entity;
    using EntityMap = std::unordered_map<UUID, Entity>;

    class Scene : public RefCounted
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const String& name = String());
        Entity CreateEntityWithID(UUID uuid, const String& name = "");
        void DestroyEntity(Entity entity);
        void DuplicateEntity(Entity entity);
        void OnUpdate(Timestep ts);
        void OnUpdateRuntime(Timestep ts);
        void OnUpdateEditor(Timestep ts, EditorCamera& camera);
        void OnViewportResize(uint32_t width, uint32_t height);
        void OnEvent(Event& e);

        // Runtime Stuff
        void OnRuntimeStart();
        void OnRuntimeStop();
        void CopySceneTo(Ref<Scene>& target);

        EntityMap GetEntityMap() { return m_EntityIDMap; }

        UUID GetUUID() const { return m_SceneID; }
        static Ref<Scene> GetScene(UUID uuid);

        /* [Spike] Entity stuff [Spike] */
        Entity GetPrimaryCameraEntity();
        Entity FindEntityByTag(const String& tag);

        template<typename T>
        auto GetAllEntitiesWith() { return m_Registry.view<T>(); }
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    public:
        EntityMap m_EntityIDMap;
        bool m_IsPlaying = false;

    private:
        UUID m_SceneID;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        entt::entity m_SceneEntity;
        entt::registry m_Registry;

        friend class Physics2D;
        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}