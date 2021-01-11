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
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/Mesh.h"
#include "Panels/ConsolePanel.h"
#include "SceneCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <FontAwesome.h>
#include "ScriptableEntity.h"

namespace Spike
{
    struct TagComponent
    {
        std::string Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string tag)
            :Tag(tag) {}

        const char* GetName() { return "Tag"; }
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
        const char* GetName() { return ICON_FK_WRENCH" Transform"; }
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
        std::string TextureFilepath;
        float TilingFactor = 1.0f;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            :Color(color) {}

        void SetTexture(const std::string& filepath)
        {
            Texture = Texture2D::Create(filepath);
            TextureFilepath = filepath;
        }

        void RemoveTexture() { Texture = nullptr; }

        void Reset()
        {
            Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            RemoveTexture();
        }
        const char* GetName() { return ICON_FK_SQUARE" Sprite Renderer"; }
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
        const char* GetName() { return ICON_FK_CAMERA" Camera"; }
    };

    struct MeshComponent
    {
        Ref<Spike::Mesh> Mesh;
        std::string MeshFilepath;

        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;

        void SetFilePath(std::string& path) { MeshFilepath = path; }
        void Reset() { Mesh = nullptr; }
        const char* GetName() { return ICON_FK_CUBE" Mesh"; }
    };

    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity* (*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
        }
        const char* GetName() { return "Native Script"; };
    };
}
