/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : SceneSerializer
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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include "Panels/ConsolePanel.h"
#include <yaml-cpp/yaml.h>
namespace YAML {

    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::quat>
    {
        static Node encode(const glm::quat& rhs)
        {
            Node node;
            node.push_back(rhs.w);
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::quat& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.w = node[0].as<float>();
            rhs.x = node[1].as<float>();
            rhs.y = node[2].as<float>();
            rhs.z = node[3].as<float>();
            return true;
        }
    };
}

namespace Spike
{

    static bool CheckPath(const String& path)
    {
        FILE* f = fopen(path.c_str(), "rb");
        if (f)
            fclose(f);
        return f != nullptr;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.w << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_Scene(scene)
    {
    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<IDComponent>())
        {
            UUID uuid = entity.GetComponent<IDComponent>().ID;
            out << YAML::BeginMap; // Entity
            out << YAML::Key << "Entity" << YAML::Value << uuid;

            if (entity.HasComponent<TagComponent>())
            {
                out << YAML::Key << "TagComponent";
                out << YAML::BeginMap; // TagComponent

                auto& tag = entity.GetComponent<TagComponent>().Tag;
                out << YAML::Key << "Tag" << YAML::Value << tag;

                out << YAML::EndMap; // TagComponent
            }

            if (entity.HasComponent<TransformComponent>())
            {
                out << YAML::Key << "TransformComponent";
                out << YAML::BeginMap; // TransformComponent

                auto& tc = entity.GetComponent<TransformComponent>();
                out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
                out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
                out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

                out << YAML::EndMap; // TransformComponent
            }

            if (entity.HasComponent<CameraComponent>())
            {
                out << YAML::Key << "CameraComponent";
                out << YAML::BeginMap; // CameraComponent

                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;

                out << YAML::Key << "Camera" << YAML::Value;
                out << YAML::BeginMap; // Camera
                out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
                out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
                out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
                out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
                out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
                out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
                out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
                out << YAML::EndMap; // Camera

                out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
                out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

                out << YAML::EndMap; // CameraComponent
            }

            if (entity.HasComponent<SpriteRendererComponent>())
            {
                out << YAML::Key << "SpriteRendererComponent";
                out << YAML::BeginMap; // SpriteRendererComponent

                auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
                out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
                out << YAML::Key << "TextureFilepath" << YAML::Value << spriteRendererComponent.TextureFilepath;
                out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
                out << YAML::EndMap; // SpriteRendererComponent
            }

            if (entity.HasComponent<MeshComponent>())
            {
                out << YAML::Key << "MeshComponent";
                out << YAML::BeginMap; // MeshComponent

                auto mesh = entity.GetComponent<MeshComponent>().Mesh;
                out << YAML::Key << "AssetPath" << YAML::Value << mesh->m_FilePath;

                out << YAML::EndMap; // MeshComponent
            }

            if (entity.HasComponent<ScriptComponent>())
            {
                out << YAML::Key << "ScriptComponent";
                out << YAML::BeginMap; // ScriptComponent

                auto moduleName = entity.GetComponent<ScriptComponent>().ModuleName;
                out << YAML::Key << "ModuleName" << moduleName;

                out << YAML::EndMap; // ScriptComponent
            }
            if (entity.HasComponent<RigidBody2DComponent>())
            {
                out << YAML::Key << "RigidBody2DComponent";
                out << YAML::BeginMap; // RigidBody2D

                auto& rb2D = entity.GetComponent<RigidBody2DComponent>();

                out << YAML::Key << "BodyType" << YAML::Value           << (int)rb2D.BodyType;
                out << YAML::Key << "FixedRotation" << YAML::Value      << rb2D.FixedRotation;
                out << YAML::Key << "Gravity" << YAML::Value            << rb2D.Gravity;
                out << YAML::Key << "CollisionDetection" << YAML::Value << (int)rb2D.CollisionDetection;
                out << YAML::Key << "Sleeptype" << YAML::Value          << (int)rb2D.Sleeptype;

                out << YAML::EndMap; // RigidBody2D
            }

            if (entity.HasComponent<BoxCollider2DComponent>())
            {
                out << YAML::Key << "BoxCollider2DComponent";
                out << YAML::BeginMap; // BoxCollider2D

                auto& boxCollider2D = entity.GetComponent<BoxCollider2DComponent>();

                out << YAML::Key << "Offset" << YAML::Value   << boxCollider2D.Offset;
                out << YAML::Key << "Size" << YAML::Value     << boxCollider2D.Size;
                out << YAML::Key << "Density" << YAML::Value  << boxCollider2D.Density;
                out << YAML::Key << "Friction" << YAML::Value << boxCollider2D.Friction;

                out << YAML::EndMap; // BoxCollider2D
            }

            if (entity.HasComponent<CircleCollider2DComponent>())
            {
                out << YAML::Key << "CircleCollider2DComponent";
                out << YAML::BeginMap; // CircleCollider2D

                auto& circleCollider2D = entity.GetComponent<CircleCollider2DComponent>();

                out << YAML::Key << "Offset" << YAML::Value   << circleCollider2D.Offset;
                out << YAML::Key << "Radius" << YAML::Value   << circleCollider2D.Radius;
                out << YAML::Key << "Density" << YAML::Value  << circleCollider2D.Density;
                out << YAML::Key << "Friction" << YAML::Value << circleCollider2D.Friction;

                out << YAML::EndMap; // CircleCollider2D
            }

            out << YAML::EndMap; // Entity
        }
    }

    void SceneSerializer::Serialize(const String& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        m_Scene->m_Registry.each([&](auto entityID)
        {
            Entity entity = { entityID, m_Scene.Raw() };
            if (!entity) return;
            SerializeEntity(out, entity);
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);

        if (fout.bad())
        {
            SPK_CORE_LOG_ERROR("Error serializing the file! Terminating serialization system...");
            return;
        }
        fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const String& filepath)
    {
        SPK_INTERNAL_ASSERT("Method not implemented yet!");
    }

    bool SceneSerializer::Deserialize(const String& filepath)
    {
        std::vector<String> missingPaths;
        YAML::Node data;
        try { data = YAML::LoadFile(filepath); }
        catch (const YAML::ParserException& ex)
        {
            SPK_CORE_LOG_ERROR("Failed to load .spike file '%s'\n     {1}", filepath.c_str(), ex.what());
        }
        if (!data["Scene"])
            return false;

        String sceneName = data["Scene"].as<String>();

        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>();

                String name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<String>();
                Entity deserializedEntity = m_Scene->CreateEntityWithID(uuid, name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    // Entities always have transforms
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>();

                    auto& cameraProps = cameraComponent["Camera"];
                    cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

                    cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
                    cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                    cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

                    cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                    cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                    cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

                    cc.Primary = cameraComponent["Primary"].as<bool>();
                    cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
                    src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
                    auto textureFilePath = spriteRendererComponent["TextureFilepath"];
                    if (textureFilePath)
                    {
                        String textureFilepath = textureFilePath.as<String>();
                        if(!textureFilepath.empty())
                            src.SetTexture(textureFilepath);
                    }
                    auto tilingFactor = spriteRendererComponent["TilingFactor"];
                    if (tilingFactor)
                        src.TilingFactor = tilingFactor.as<float>();
                }

                auto meshComponent = entity["MeshComponent"];
                if (meshComponent)
                {
                    String meshPath = meshComponent["AssetPath"].as<String>();

                    if (!deserializedEntity.HasComponent<MeshComponent>())
                    {
                        Ref<Mesh> mesh;
                        if (!CheckPath(meshPath))
                            missingPaths.emplace_back(meshPath);
                        else
                            mesh = Ref<Mesh>::Create(meshPath);

                        deserializedEntity.AddComponent<MeshComponent>(mesh);
                    }

                    SPK_CORE_LOG_INFO("  Mesh Asset Path: %s", meshPath.c_str());
                }

                auto scriptComponent = entity["ScriptComponent"];
                if (scriptComponent)
                {
                    String moduleName = scriptComponent["ModuleName"].as<String>();

                    if (!deserializedEntity.HasComponent<ScriptComponent>())
                    {
                        deserializedEntity.AddComponent<ScriptComponent>(moduleName);
                    }
                }

                auto rigidBody2DComponent = entity["RigidBody2DComponent"];
                if (rigidBody2DComponent)
                {
                    if (!deserializedEntity.HasComponent<RigidBody2DComponent>())
                    {
                        auto& component = deserializedEntity.AddComponent<RigidBody2DComponent>();
                        component.BodyType = (RigidBody2DComponent::Type)rigidBody2DComponent["BodyType"].as<int>();
                        component.FixedRotation = rigidBody2DComponent["FixedRotation"] ? rigidBody2DComponent["FixedRotation"].as<bool>() : false;
                        component.Gravity = rigidBody2DComponent["Gravity"].as<float>();
                        component.CollisionDetection = (CollisionDetectionType)rigidBody2DComponent["CollisionDetection"].as<int>();
                        component.Sleeptype = (SleepType)rigidBody2DComponent["Sleeptype"].as<int>();
                    }
                }

                auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
                if (boxCollider2DComponent)
                {
                    if (!deserializedEntity.HasComponent<BoxCollider2DComponent>())
                    {
                        auto& component = deserializedEntity.AddComponent<BoxCollider2DComponent>();
                        component.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
                        component.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
                        component.Density = boxCollider2DComponent["Density"] ? boxCollider2DComponent["Density"].as<float>() : 1.0f;
                        component.Friction = boxCollider2DComponent["Friction"] ? boxCollider2DComponent["Friction"].as<float>() : 1.0f;
                    }
                }

                auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
                if (circleCollider2DComponent)
                {
                    if (!deserializedEntity.HasComponent<CircleCollider2DComponent>())
                    {
                        auto& component = deserializedEntity.AddComponent<CircleCollider2DComponent>();
                        component.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
                        component.Radius = circleCollider2DComponent["Radius"].as<float>();
                        component.Density = circleCollider2DComponent["Density"] ? circleCollider2DComponent["Density"].as<float>() : 1.0f;
                        component.Friction = circleCollider2DComponent["Friction"] ? circleCollider2DComponent["Friction"].as<float>() : 1.0f;
                    }
                }
            }
        }

        if (missingPaths.size())
        {
            SPK_CORE_LOG_ERROR("The following files could not be loaded:");
            for (auto& path : missingPaths)
            {
                SPK_CORE_LOG_ERROR("  %s", path.c_str());
            }

            return false;
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const String& filepath)
    {
        SPK_INTERNAL_ASSERT("Method not implemented yet!");
        return false;
    }

}