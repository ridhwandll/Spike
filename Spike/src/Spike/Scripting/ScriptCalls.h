/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptCalls
File Type      : h
File created on: 2021/01/26
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Core/Input.h"
#include "Spike/Scene/Components.h"
#include <glm/glm.hpp>
#include <mono/metadata/object.h>

namespace Spike::Scripting
{
    /* [Spike] Console [Spike] */
    void Spike_Console_LogInfo(MonoString* message);
    void Spike_Console_LogWarn(MonoString* message);
    void Spike_Console_LogDebug(MonoString* message);
    void Spike_Console_LogError(MonoString* message);
    void Spike_Console_LogCritical(MonoString* message);

    /* [Spike] Input [Spike] */
    bool Spike_Input_IsKeyPressed(KeyCode key);
    bool Spike_Input_IsMouseButtonPressed(MouseCode button);
    void Spike_Input_GetMousePosition(glm::vec2* outPosition);
    void Spike_Input_SetCursorMode(MousePointerMode mode);
    MousePointerMode Spike_Input_GetCursorMode();

    /* [Spike] Entity [Spike] */
    void Spike_Entity_CreateComponent(uint64_t entityID, void* type);
    bool Spike_Entity_HasComponent(uint64_t entityID, void* type);
    uint64_t Spike_Entity_FindEntityByTag(MonoString* tag);

    /* [Spike] Tag Component [Spike] */
    MonoString* Spike_TagComponent_GetTag(uint64_t entityID);
    void Spike_TagComponent_SetTag(uint64_t entityID, MonoString* inTag);

    /* [Spike] Transform Component [Spike] */
    void Spike_TransformComponent_GetTransform(uint64_t entityID, TransformComponent* outTransform);
    void Spike_TransformComponent_SetTransform(uint64_t entityID, TransformComponent* inTransform);
    void Spike_TransformComponent_GetTranslation(uint64_t entityID, glm::vec3* outTranslation);
    void Spike_TransformComponent_SetTranslation(uint64_t entityID, glm::vec3* inTranslation);
    void Spike_TransformComponent_GetRotation(uint64_t entityID, glm::vec3* outRotation);
    void Spike_TransformComponent_SetRotation(uint64_t entityID, glm::vec3* inRotation);
    void Spike_TransformComponent_GetScale(uint64_t entityID, glm::vec3* outScale);
    void Spike_TransformComponent_SetScale(uint64_t entityID, glm::vec3* inScale);

    /* [Spike] RigidBody2D Component [Spike] */
    void Spike_RigidBody2DComponent_ApplyLinearImpulse(uint64_t entityID, glm::vec2* impulse, glm::vec2* offset, bool wake);
    void Spike_RigidBody2DComponent_GetLinearVelocity(uint64_t entityID, glm::vec2* outVelocity);
    void Spike_RigidBody2DComponent_SetLinearVelocity(uint64_t entityID, glm::vec2* velocity);

    /* [Spike] SpriteRenderer Component [Spike] */
    void Spike_SpriteRendererComponent_GetColor(uint64_t entityID, glm::vec4* outColor);
    void Spike_SpriteRendererComponent_SetColor(uint64_t entityID, glm::vec4* inColor);

    /* [Spike] Camera Component [Spike] */
    void Spike_CameraComponent_SetAsPrimary(uint64_t entityID, bool* isPrimary);
    bool Spike_CameraComponent_IsPrimary(uint64_t entityID);
    void Spike_CameraComponent_SetFixedAspectRatio(uint64_t entityID, bool isAspectRatioFixed);
    bool Spike_CameraComponent_IsFixedAspectRatio(uint64_t entityID);
} 