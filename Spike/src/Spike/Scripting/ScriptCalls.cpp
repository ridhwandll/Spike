/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptCalls
File Type      : cpp
File created on: 2021/01/26
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Spike/Scene/Entity.h"
#include "ScriptCalls.h"
#include "Panels/ConsolePanel.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>

namespace Spike
{
    extern std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
    extern std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;
}

namespace Spike::Scripting
{
    std::string ConvertMonoString(MonoString* message)
    {
        /* [Spike] Any way to make this efficient? we are doing 2 copies here! [Spike] */
        char* ptr = mono_string_to_utf8(message);
        std::string s { ptr };
        mono_free(ptr);
        return s;
    }

    void Spike_Console_LogInfo(MonoString* message)
    {
        SPK_CORE_LOG_INFO(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_INFO);
    }

    void Spike_Console_LogWarn(MonoString* message)
    {
        SPK_CORE_LOG_WARN(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_WARN);
    }

    void Spike_Console_LogDebug(MonoString* message)
    {
        SPK_CORE_LOG_DEBUG(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_DEBUG);
    }

    void Spike_Console_LogError(MonoString* message)
    {
        SPK_CORE_LOG_ERROR(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_ERROR);
    }

    void Spike_Console_LogCritical(MonoString* message)
    {
        SPK_CORE_LOG_CRITICAL(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_CRITICAL);
    }

    /* [Spike] INPUT [Spike] */
    bool Spike_Input_IsKeyPressed(KeyCode key)
    {
        return Spike::Input::IsKeyPressed(key);
    }

    bool Spike_Input_IsMouseButtonPressed(MouseCode button)
    {
        return Spike::Input::IsMouseButtonPressed(button);
    }

    void Spike_Input_GetMousePosition(glm::vec2* outPosition)
    {
        glm::vec2 result =  Spike::Input::GetMousePosition();
        *outPosition = result;
    }

    void Spike_Input_SetCursorMode(MousePointerMode mode)
    {
        Spike::Input::SetCursorMode(mode);
    }

    MousePointerMode Spike_Input_GetCursorMode()
    {
        return Spike::Input::GetCursorMode();
    }

    /* [Spike] ENTITY [Spike] */
    void Spike_Entity_CreateComponent(uint64_t entityID, void* type)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity id is invalid!");

        Entity entity = entityMap.at(entityID);
        MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
        s_CreateComponentFuncs[monoType](entity);
    }

    bool Spike_Entity_HasComponent(uint64_t entityID, void* type)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
        bool result = s_HasComponentFuncs[monoType](entity);
        return result;
    }

    uint64_t Spike_Entity_FindEntityByTag(MonoString* tag)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");

        Entity entity = scene->FindEntityByTag(mono_string_to_utf8(tag));
        if (entity)
            return entity.GetComponent<IDComponent>().ID;

        return 0;
    }

    /* [Spike] TRANSFORM COMPONENT [Spike] */

    void Spike_TransformComponent_GetTransform(uint64_t entityID, TransformComponent* outTransform)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        *outTransform = entity.GetComponent<TransformComponent>();
    }

    void Spike_TransformComponent_SetTransform(uint64_t entityID, TransformComponent* inTransform)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        entity.GetComponent<TransformComponent>() = *inTransform;
    }

    void Spike_TransformComponent_GetTranslation(uint64_t entityID, glm::vec3* outTranslation)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        *outTranslation = entity.GetComponent<TransformComponent>().Translation;
    }

    void Spike_TransformComponent_SetTranslation(uint64_t entityID, glm::vec3* inTranslation)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        entity.GetComponent<TransformComponent>().Translation = *inTranslation;
    }

    void Spike_TransformComponent_GetRotation(uint64_t entityID, glm::vec3* outRotation)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        *outRotation = entity.GetComponent<TransformComponent>().Rotation;
    }

    void Spike_TransformComponent_SetRotation(uint64_t entityID, glm::vec3* inRotation)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        entity.GetComponent<TransformComponent>().Rotation = *inRotation;
    }

    void Spike_TransformComponent_GetScale(uint64_t entityID, glm::vec3* outScale)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        *outScale = entity.GetComponent<TransformComponent>().Scale;
    }

    void Spike_TransformComponent_SetScale(uint64_t entityID, glm::vec3* inScale)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");
        const auto& entityMap = scene->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");

        Entity entity = entityMap.at(entityID);
        entity.GetComponent<TransformComponent>().Scale = *inScale;
    }

}
