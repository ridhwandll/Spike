#include "lepch.h"
#include "Scene.h"
#include "LightEngine/Renderer/Renderer2D.h"
#include <glm/glm.hpp>
#include "LightEngine/Scene/Components.h"

namespace LightEngine
{
    struct MeshComponent 
    {
        float Value;
        MeshComponent() = default;
    };
    Scene::Scene()
    {
#if 0
        entt::entity entity = m_Registry.create();

        if (m_Registry.has<TransformComponent>(entity))
            m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

        auto view = m_Registry.view<TransformComponent>();
        for (auto entity : view)
        {
            TransformComponent& transform = view.get<TransformComponent>(entity);
        }
#endif
    }

    Scene::~Scene()
    {

    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();
    }

    void Scene::OnUpdate(Timestep ts)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : group)
        {
            auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform, sprite.Color);
        }
    }

}