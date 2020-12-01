#include "lepch.h"
#include "Entity.h"

namespace LightEngine
{
    Entity::Entity(entt::entity handle, Scene* scene)
        :m_EntityHandle(handle), m_Scene(scene)
    {

    }

}