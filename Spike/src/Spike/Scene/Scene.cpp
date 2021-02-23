/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Scene
File Type      : cpp
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
#include "spkpch.h"
#include "Scene.h"
#include "Spike/Renderer/Renderer2D.h"
#include "Spike/Renderer/Renderer.h"
#include "Spike/Scene/Components.h"
#include "Spike/Core/Input.h"
#include "Spike/Physics/2D/Physics2D.h"
#include "Spike/Scripting/ScriptEngine.h"
#include "Entity.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Spike
{
    std::unordered_map<UUID, Scene*> s_ActiveScenes;

    struct SceneComponent
    {
        UUID SceneID;
    };

    static void OnScriptComponentConstruct(entt::registry& registry, entt::entity entity)
    {
        auto sceneView = registry.view<SceneComponent>();
        UUID sceneID = registry.get<SceneComponent>(sceneView.front()).SceneID;

        Scene* scene = s_ActiveScenes[sceneID];

        auto entityID = registry.get<IDComponent>(entity).ID;
        SPK_CRIRICAL(scene->m_EntityIDMap.find(entityID) != scene->m_EntityIDMap.end(), "");
        ScriptEngine::InitScriptEntity(scene->m_EntityIDMap.at(entityID));
    }

    static void OnScriptComponentDestroy(entt::registry& registry, entt::entity entity)
    {
        auto sceneView = registry.view<SceneComponent>();
        UUID sceneID = registry.get<SceneComponent>(sceneView.front()).SceneID;

        Scene* scene = s_ActiveScenes[sceneID];

        auto entityID = registry.get<IDComponent>(entity).ID;
        ScriptEngine::OnScriptComponentDestroyed(sceneID, entityID);
    }

    Scene::Scene()
    {
        m_Registry.on_construct<ScriptComponent>().connect<&OnScriptComponentConstruct>();

        m_SceneEntity = m_Registry.create();
        s_ActiveScenes[m_SceneID] = this;

        m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID);
        Physics2D::CreateScene(this);
    }

    Scene::~Scene()
    {
        m_Registry.on_destroy<ScriptComponent>().disconnect();
        ScriptEngine::OnSceneDestruct(m_SceneID);
        m_Registry.clear();
        s_ActiveScenes.erase(m_SceneID);
    }

    Entity Scene::CreateEntity(const String& name)
    {
        auto entity = Entity{ m_Registry.create(), this };
        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = {};

        entity.AddComponent<TransformComponent>();
        if (!name.empty())
            entity.AddComponent<TagComponent>(name);

        m_EntityIDMap[idComponent.ID] = entity;
        return entity;
    }

    Entity Scene::CreateEntityWithID(UUID uuid, const String& name, bool runtimeMap)
    {
        auto entity = Entity{ m_Registry.create(), this };
        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = uuid;

        entity.AddComponent<TransformComponent>();
        if (!name.empty())
            entity.AddComponent<TagComponent>(name);

        SPK_CORE_ASSERT(m_EntityIDMap.find(uuid) == m_EntityIDMap.end(), "Entity with the given id already exists!");
        m_EntityIDMap[uuid] = entity;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        if (entity.HasComponent<ScriptComponent>())
            ScriptEngine::OnScriptComponentDestroyed(m_SceneID, entity.GetUUID());

        m_Registry.destroy(entity);
    }

    void Scene::OnUpdate(Timestep ts)
    {
        Physics2D::Simulate(ts);
    }

    void Scene::OnUpdateRuntime(Timestep ts)
    {
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;

        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }
        if (mainCamera)
        {
            {
                Renderer2D::BeginScene(*mainCamera, cameraTransform);
                auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for (auto entity : group)
                {
                    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                    Renderer2D::DrawSprite(transform.GetTransform(), sprite);
                }

                Renderer2D::EndScene();
            }
            {
                Renderer::BeginScene(*mainCamera, cameraTransform);
                auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
                for (auto entity : group)
                {
                    auto [mesh, transform] = group.get<MeshComponent, TransformComponent>(entity);
                    if (mesh.Mesh)
                    {
                        Renderer::SubmitMesh(mesh.Mesh, transform.GetTransform());
                    }
                }
                Renderer::EndScene();
            }
        }

        // Update scripts
        {
            auto view = m_Registry.view<ScriptComponent>();
            for (auto entity : view)
            {
                Entity e = { entity, this };
                if (ScriptEngine::ModuleExists(e.GetComponent<ScriptComponent>().ModuleName))
                {
                    ScriptEngine::OnUpdateEntity(e, ts);
                    /* [Spike] Maybe not hardcode the fixed timestep and have a settings struct? [Spike] */
                    ScriptEngine::OnFixedUpdateEntity(e, 0.02f); //0.02f is hardcoded here
                }
            }
        }
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
    {
        {
            Renderer2D::BeginScene(camera);

            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawSprite(transform.GetTransform(), sprite);
            }

            auto view = m_Registry.view<TransformComponent, BoxCollider2DComponent>();
            const glm::vec4 debugColor(0.5f, 0.9f, 0.5f, 0.25f);
            for (auto entity : view)
            {
                auto[transformComponent, boxCollider] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
                glm::mat4 trans = transformComponent.GetTransform() *
                    glm::translate(glm::mat4(1.0f), glm::vec3(boxCollider.Offset.x, boxCollider.Offset.y, 0.0f)) * 
                    glm::scale(glm::mat4(1.0f), glm::vec3(boxCollider.Size.x, boxCollider.Size.y, 1.0f));

                Renderer2D::DrawQuad(trans, debugColor);
            }

            Renderer2D::EndScene();
        }

        {
            Renderer::BeginScene(camera);
            auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
            for (auto entity : group)
            {
                auto [mesh, transform] = group.get<MeshComponent, TransformComponent>(entity);
                if (mesh.Mesh)
                {
                    Renderer::SubmitMesh(mesh.Mesh, transform.GetTransform(), (int)entity);
                }
            }
            Renderer::EndScene();
        }
    }

    template<typename T>
    static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
    {
        auto components = srcRegistry.view<T>();
        for (auto srcEntity : components)
        {
            entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);

            auto& srcComponent = srcRegistry.get<T>(srcEntity);
            auto& destComponent = dstRegistry.emplace_or_replace<T>(destEntity, srcComponent);
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.SetViewportSize(width, height);
        }
    }

    void Scene::OnEvent(Event& e) { }

    void Scene::OnRuntimeStart()
    {
        ScriptEngine::SetSceneContext(this);
        {
            auto view = m_Registry.view<ScriptComponent>();
            for (auto entity : view)
            {
                Entity e = { entity, this };
                if (ScriptEngine::ModuleExists(e.GetComponent<ScriptComponent>().ModuleName))
                    ScriptEngine::InstantiateEntityClass(e);
            }
        }

        {
            auto view = m_Registry.view<TransformComponent, BoxCollider2DComponent>();
            for (auto entity : view)
            {
                auto [transform, boxCollider2D] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
                boxCollider2D.Scale = transform.Scale;
            }
        }
        Physics2D::Init();
        m_IsPlaying = true;
    }

    void Scene::OnRuntimeStop()
    {
        Physics2D::Shutdown();
        m_IsPlaying = false;
    }

    void Scene::CopySceneTo(Ref<Scene>& target)
    {
        std::unordered_map<UUID, entt::entity> enttMap;
        auto idComponents = m_Registry.view<IDComponent>();
        for (auto entity : idComponents)
        {
            auto uuid = m_Registry.get<IDComponent>(entity).ID;
            Entity e = target->CreateEntityWithID(uuid, "", true);
            enttMap[uuid] = e.Raw();
        }

        CopyComponent<TagComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<TransformComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<MeshComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<CameraComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<SpriteRendererComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<ScriptComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<RigidBody2DComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<BoxCollider2DComponent>(target->m_Registry, m_Registry, enttMap);
        CopyComponent<CircleCollider2DComponent>(target->m_Registry, m_Registry, enttMap);


        const auto& entityInstanceMap = ScriptEngine::GetEntityInstanceMap();
        if (entityInstanceMap.find(target->GetUUID()) != entityInstanceMap.end())
            ScriptEngine::CopyEntityScriptData(target->GetUUID(), m_SceneID);
    }

    template<typename T>
    static void CopyComponentIfExists(entt::entity dst, entt::entity src, entt::registry& registry)
    {
        if (registry.has<T>(src))
        {
            auto& srcComponent = registry.get<T>(src);
            registry.emplace_or_replace<T>(dst, srcComponent);
        }
    }

    void Scene::DuplicateEntity(Entity entity)
    {
        Entity newEntity;
        if (entity.HasComponent<TagComponent>())
            newEntity = CreateEntity(entity.GetComponent<TagComponent>().Tag);
        else
            newEntity = CreateEntity();

        CopyComponentIfExists<TransformComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
        CopyComponentIfExists<MeshComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
        CopyComponentIfExists<CameraComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
        CopyComponentIfExists<SpriteRendererComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
        CopyComponentIfExists<ScriptComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
        CopyComponentIfExists<RigidBody2DComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
        CopyComponentIfExists<BoxCollider2DComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
        CopyComponentIfExists<CircleCollider2DComponent>(newEntity.m_EntityHandle, entity.m_EntityHandle, m_Registry);
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.Primary)
                return Entity{ entity, this };
        }
        return {};
    }

    Entity Scene::FindEntityByTag(const String& tag)
    {
        auto view = m_Registry.view<TagComponent>();
        for (auto entity : view)
        {
            const auto& theRealTag = view.get<TagComponent>(entity).Tag;
            if (theRealTag == tag)
                return Entity{ entity, this };
        }
        return {};
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }

    Ref<Scene> Scene::GetScene(UUID uuid)
    {
        if (s_ActiveScenes.find(uuid) != s_ActiveScenes.end())
            return s_ActiveScenes.at(uuid);

        return {};
    }

    template<>
    void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
    {

    }
}