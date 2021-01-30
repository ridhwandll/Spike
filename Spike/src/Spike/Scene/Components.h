/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Components
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
#include "Spike/Core/UUID.h"
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/Mesh.h"
#include "Panels/ConsolePanel.h"
#include "SceneCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <FontAwesome.h>

namespace Spike
{
    struct IDComponent
    {
        UUID ID = 0;
    };

    struct TagComponent
    {
        String Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const String tag)
            :Tag(tag) {}
    };

    struct TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
            :Translation(translation) {}

        void Reset()
        {
            Translation = { 0.0f, 0.0f, 0.0f };
            Rotation = { 0.0f, 0.0f, 0.0f };
            Scale = { 1.0f, 1.0f, 1.0f };
        }
        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> Texture = nullptr;
        String TextureFilepath;
        float TilingFactor = 1.0f;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            :Color(color) {}

        void SetTexture(const String& filepath)
        {
            Texture = Texture2D::Create(filepath);
            TextureFilepath = filepath;
        }

        void RemoveTexture() { Texture = nullptr; TextureFilepath = ""; }

        void Reset()
        {
            Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            RemoveTexture();
        }
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        void Reset()
        {
            Primary = true;
            FixedAspectRatio = false;
        }
    };

    struct MeshComponent
    {
        Ref<Spike::Mesh> Mesh;
        String MeshFilepath;

        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;

        void SetFilePath(String& path) { MeshFilepath = path; }
        void Reset() { Mesh = nullptr; }
    };

    struct ScriptComponent
    {
        String ModuleName;

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
        void Reset() { ModuleName = "SpikeNull"; }
    };

    //// 2D Physics //////////////////////////////////////////////////////////////////////
    enum class CollisionDetectionType;
    enum class SleepType;

    struct RigidBody2DComponent
    {
        enum class Type { Static, Dynamic, Kinematic };

        Type BodyType;
        bool FixedRotation = false;
        float Gravity = 1.0f;

        void* RuntimeBody = nullptr; //Needed at runtime
        CollisionDetectionType CollisionDetection;
        SleepType Sleeptype;

        RigidBody2DComponent() = default;
        RigidBody2DComponent(const RigidBody2DComponent & other) = default;

        void Reset()
        {
            BodyType = Type::Static; Gravity = 1.0f;
        }
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 1.0f, 1.0f };
        glm::vec2 Scale = { 1.0f, 1.0f };

        float Density = 1.0f;
        float Friction = 1.0f;

        void* RuntimeFixture = nullptr; //Needed at runtime

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent & other) = default;

        void Reset()
        {
            Scale = { 1.0f, 1.0f };
            Offset = { 0.0f, 0.0f };
            Size = { 1.0f, 1.0f };

            Density = 1.0f;
            Friction = 1.0f;
        }
    };

    struct CircleCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 1.0f;

        float Density = 1.0f;
        float Friction = 1.0f;

        void* RuntimeFixture = nullptr;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent& other) = default;

        void Reset()
        {
            Offset = { 0.0f, 0.0f };
            Radius = 1.0f;

            Density = 1.0f;
            Friction = 1.0f;
        }
    };
    /////2D Physics End///////////////////////////////////////////////////////////////////
}
