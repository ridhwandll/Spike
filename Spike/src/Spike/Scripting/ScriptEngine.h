/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptEngine
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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Scene/Components.h"
#include "Spike/Scene/Entity.h"

extern "C"
{
    /* [Spike] We dont want to include the big mono header files here 🙂[Spike] */
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoClassField MonoClassField;
    typedef struct _MonoClass MonoClass;
}

namespace Spike
{
    enum class FieldType
    {
        None = 0, Float = 1, Int = 2, UnsignedInt = 3, String = 4, Vec2 = 5, Vec3 = 6, Vec4 = 7
    };

    const char* FieldTypeToString(FieldType type);

    struct EntityScriptClass;
    struct EntityInstance
    {
        EntityScriptClass* ScriptClass;
        uint32_t Handle = 0;
        /* [Spike] This class represents a single Entity [Spike] */
        Scene* SceneInstance = nullptr;
        MonoObject* GetInstance();
    };

    struct PublicField
    {
        std::string Name;
        FieldType Type;

        PublicField(const std::string& name, FieldType type);
        PublicField(const PublicField&) = delete;
        PublicField(PublicField&& other);
        ~PublicField();

        void CopyStoredValueToRuntime();
        bool IsRuntimeAvailable() const;

        template<typename T>
        T GetStoredValue() const
        {
            T value;
            GetStoredValue_Internal(&value);
            return value;
        }

        template<typename T>
        void SetStoredValue(T value) const
        {
            SetStoredValue_Internal(&value);
        }

        template<typename T>
        T GetRuntimeValue() const
        {
            T value;
            GetRuntimeValue_Internal(&value);
            return value;
        }

        template<typename T>
        void SetRuntimeValue(T value) const
        {
            SetRuntimeValue_Internal(&value);
        }

        void SetStoredValueRaw(void* src);
    private:
        EntityInstance* m_EntityInstance;
        MonoClassField* m_MonoClassField;
        uint8_t* m_StoredValueBuffer = nullptr;

        uint8_t* AllocateBuffer(FieldType type);
        void SetStoredValue_Internal(void* value) const;
        void GetStoredValue_Internal(void* outValue) const;
        void SetRuntimeValue_Internal(void* value) const;
        void GetRuntimeValue_Internal(void* outValue) const;

        friend class ScriptEngine;
    };

    using ScriptModuleFieldMap = std::unordered_map<std::string, std::unordered_map<std::string, PublicField>>;

    struct EntityInstanceData
    {
        EntityInstance Instance;
        ScriptModuleFieldMap ModuleFieldMap;
    };

    /* [Spike] Mapped as-> SceneID --- (EntityID - InstanceData) [Spike] */
    using EntityInstanceMap = std::unordered_map<UUID, std::unordered_map<UUID, EntityInstanceData>>;

    /* [Spike] The whole Script Engine, initialized at Application.cpp [Spike] */
    class ScriptEngine
    {
    public:
        /* [Spike] Initialize the Script Engine [Spike] */
        static void Init(const std::string& assemblyPath);
        static void Shutdown();

        static void OnCreateEntity(Entity entity);
        static void OnUpdateEntity(Entity entity, Timestep ts);

        /* [Spike] Script Engine must have a scene to work on, we set the scene context by this [Spike] */
        static void SetSceneContext(const Ref<Scene>& scene);
        static Ref<Scene> GetSceneContext();

        static MonoObject* Construct(const std::string& fullName, bool callConstructor = true, void** parameters = nullptr);
        static MonoClass* GetCoreClass(const std::string& fullName);

        static bool IsEntityModuleValid(Entity entity);
        static void OnSceneDestruct(UUID sceneID);
        static void OnScriptComponentDestroyed(UUID sceneID, UUID entityID);
        static void LoadSpikeRuntimeAssembly(const std::string& path);
        static void ReloadAssembly(const std::string& path);

        static bool ModuleExists(const std::string& moduleName);
        static void InitScriptEntity(Entity entity);
        static void ShutdownScriptEntity(Entity entity, const std::string& moduleName);
        static void InstantiateEntityClass(Entity entity);
        static void CopyEntityScriptData(UUID dst, UUID src);

        static EntityInstanceMap& GetEntityInstanceMap();
        static EntityInstanceData& GetEntityInstanceData(UUID sceneID, UUID entityID);

        /* [Spike] Debug Only [Spike] */
        static void OnImGuiRender();
    };
}