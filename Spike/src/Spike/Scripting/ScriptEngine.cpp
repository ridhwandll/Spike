/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptEngine
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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "ScriptEngine.h"
#include "ScriptRegistry.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>

namespace Spike
{
    static MonoMethod* GetMethod(MonoImage* image, const String& methodDesc);
    MonoAssembly* LoadAssemblyFromFile(const char* filepath);

    static MonoDomain* s_MonoDomain = nullptr;
    static Ref<Scene> s_SceneContext;
    static String s_AssemblyPath;

    static MonoAssembly* s_CoreAssembly;
    static MonoAssembly* s_AppAssembly;

    /* [Spike] Assembly Images [Spike] */
    MonoImage* s_AppAssemblyImage = nullptr;
    MonoImage* s_CoreAssemblyImage = nullptr;

    static EntityInstanceMap s_EntityInstanceMap;
    static std::unordered_map<String, MonoClass*> s_Classes;
    static std::unordered_map<String, EntityScriptClass> s_EntityClassMap;


    struct EntityScriptClass
    {
        String FullName;
        String ClassName;
        String NamespaceName;

        /* [Spike] Represents a script class in the C# script [Spike] */
        MonoClass* Class = nullptr;
        MonoMethod* Constructor = nullptr;
        MonoMethod* OnStartMethod = nullptr;
        MonoMethod* OnDestroyMethod = nullptr;
        MonoMethod* OnUpdateMethod = nullptr;
        MonoMethod* OnFixedUpdateMethod = nullptr;

        // Physics stuff
        MonoMethod* OnCollision2DBeginMethod = nullptr;
        MonoMethod* OnCollision2DEndMethod = nullptr;

        /* [Spike] TODO: Physics methods and Fixed Update [Spike] */

        void InitClassMethods(MonoImage* image)
        {
            Constructor = GetMethod(s_CoreAssemblyImage, "Spike.Entity:.ctor(ulong)");
            OnStartMethod = GetMethod(image, FullName + ":Start()");
            OnUpdateMethod = GetMethod(image, FullName + ":Update(single)");
            OnFixedUpdateMethod = GetMethod(image, FullName + ":FixedUpdate(single)");
            /* [Spike] TODO: On Destroy [Spike] */

            OnCollision2DBeginMethod = GetMethod(s_CoreAssemblyImage, "Spike.Entity:OnCollision2DBegin(single)");
            OnCollision2DEndMethod = GetMethod(s_CoreAssemblyImage, "Spike.Entity:OnCollision2DEnd(single)");
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Static Helper Functions for the ScriptEngine starts here. Maybe port this section to it's own file? */

    /* [Spike] Initialized the Mono [Spike] */
    static void InitMono()
    {
        mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc"); //TODO: Remove
        mono_set_assemblies_path("Spike/vendor/mono/lib");
        MonoDomain* domain = mono_jit_init("Spike");

        /* [Spike] Register an app domain [Spike] */
        char* name = (char*)"Spike-Runtime";
        s_MonoDomain = mono_domain_create_appdomain(name, nullptr);
    }

    /* [Spike] Shutdowns the Mono [Spike] */
    static void ShutdownMono()
    {
        mono_jit_cleanup(s_MonoDomain);
    }

    /* [Spike] Gets the Method defined in the C# script [Spike] */
    static MonoMethod* GetMethod(MonoImage* image, const String& methodDesc)
    {
        MonoMethodDesc* description = mono_method_desc_new(methodDesc.c_str(), NULL);
        if (!description)
            SPK_CORE_LOG_ERROR("mono_method_desc_new failed!");

        MonoMethod* method = mono_method_desc_search_in_image(description, image);
        if (!method)
            SPK_CORE_LOG_WARN("mono_method_desc_search_in_image failed!");
        return method;
    }

    /* [Spike] Calls a method in the C# [Spike] */
    static MonoObject* CallMethod(MonoObject* object, MonoMethod* method, void** params = nullptr)
    {
        MonoObject* pException = nullptr;
        MonoObject* result = mono_runtime_invoke(method, object, params, &pException);
        return result;
    }

    /* [Spike] Gets a class from the Image! 🔎 [Spike] */
    static MonoClass* GetClass(MonoImage* image, const EntityScriptClass& scriptClass)
    {
        MonoClass* monoClass = mono_class_from_name(image, scriptClass.NamespaceName.c_str(), scriptClass.ClassName.c_str());
        if (!monoClass) SPK_CORE_LOG_ERROR("mono_class_from_name failed");
        return monoClass;
    }

    /* [Spike] A conversion from MonoType to native Field Type [Spike] */
    static FieldType MonoTypeToSpikeFieldType(MonoType* monoType)
    {
        int type = mono_type_get_type(monoType);
        switch (type)
        {
            case MONO_TYPE_R4: return FieldType::Float;
            case MONO_TYPE_I4: return FieldType::Int;
            case MONO_TYPE_U4: return FieldType::UnsignedInt;
            case MONO_TYPE_STRING: return FieldType::_String;
            case MONO_TYPE_VALUETYPE:
            {
                char* name = mono_type_get_name(monoType);
                if (strcmp(name, "Spike.Vector2") == 0) return FieldType::Vec2;
                if (strcmp(name, "Spike.Vector3") == 0) return FieldType::Vec3;
                if (strcmp(name, "Spike.Vector4") == 0) return FieldType::Vec4;
            }
        }
        return FieldType::None;
    }

    /* [Spike] Instantiate the Mono Class [Spike] */
    static uint32_t Instantiate(EntityScriptClass& scriptClass)
    {
        MonoObject* instance = mono_object_new(s_MonoDomain, scriptClass.Class);
        if (!instance) SPK_CORE_LOG_ERROR("mono_object_new failed");

        mono_runtime_object_init(instance);
        uint32_t handle = mono_gchandle_new(instance, false);
        return handle;
    }

    /* [Spike] Gets the Image from the Assembly [Spike] */
    static MonoImage* GetAssemblyImage(MonoAssembly* assembly)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        if (!image)
            SPK_CORE_LOG_ERROR("mono_assembly_get_image failed");

        return image;
    }

    /* [Spike] Loads the Assembly [Spike] */
    static MonoAssembly* LoadAssembly(const String& path)
    {
        MonoAssembly* assembly = LoadAssemblyFromFile(path.c_str());
        if (!assembly)
            SPK_CORE_LOG_CRITICAL("Could not load assembly: %s", path.c_str());

        return assembly;
    }
    /* Static Helper Functions for the ScriptEngine ends here                                              */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    MonoAssembly* LoadAssemblyFromFile(const char* filepath)
    {
        Vector<char> fileData = Vault::ReadBinaryFile(filepath);
        MonoImageOpenStatus status;

        MonoImage* image = mono_image_open_from_data_full(fileData.data(), fileData.size(), 1, &status, 0);
        if (status != MONO_IMAGE_OK)
            return NULL;

        auto assemb = mono_assembly_load_from_full(image, filepath, &status, 0);
        mono_image_close(image);
        return assemb;
    }

    const char* FieldTypeToString(FieldType type)
    {
        switch (type)
        {
            case FieldType::None:        return "None";
            case FieldType::Float:       return "Float";
            case FieldType::Int:         return "Int";
            case FieldType::UnsignedInt: return "UnsignedInt";
            case FieldType::_String:     return "String";
            case FieldType::Vec2:        return "Vec2";
            case FieldType::Vec3:        return "Vec3";
            case FieldType::Vec4:        return "Vec4";
        }
        return "Unknown Type";
    }

    void ScriptEngine::Init(const String& assemblyPath)
    {
        s_AssemblyPath = assemblyPath;
        InitMono();
        LoadSpikeRuntimeAssembly(assemblyPath);
    }

    void ScriptEngine::Shutdown()
    {
        ShutdownMono();
        s_SceneContext = nullptr;
    }

    void ScriptEngine::OnStartEntity(Entity entity)
    {
        EntityInstance& entityInstance = GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID()).Instance;
        if (entityInstance.ScriptClass->OnStartMethod)
            CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnStartMethod);
    }

    void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
    {
        EntityInstance& entityInstance = GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID()).Instance;
        if (entityInstance.ScriptClass->OnUpdateMethod)
        {
            void* args[] = { &ts };
            CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnUpdateMethod, args);
        }
    }

    void ScriptEngine::OnFixedUpdateEntity(Entity entity, float fixedTimeStep)
    {
        EntityInstance& entityInstance = GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID()).Instance;
        if (entityInstance.ScriptClass->OnFixedUpdateMethod)
        {
            void* args[] = { &fixedTimeStep };
            CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnFixedUpdateMethod, args);
        }
    }

    void ScriptEngine::OnCollision2DBegin(Entity entity)
    {
        EntityInstance& entityInstance = GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID()).Instance;
        if (entityInstance.ScriptClass->OnCollision2DBeginMethod)
        {
            float value = 5.0f;
            void* args[] = { &value };
            CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnCollision2DBeginMethod, args);
        }
    }

    void ScriptEngine::OnCollision2DEnd(Entity entity)
    {
        EntityInstance& entityInstance = GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID()).Instance;
        if (entityInstance.ScriptClass->OnCollision2DEndMethod)
        {
            float value = 5.0f;
            void* args[] = { &value };
            CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnCollision2DEndMethod, args);
        }
    }

    void ScriptEngine::SetSceneContext(const Ref<Scene>& scene) { s_SceneContext = scene; }
    Ref<Spike::Scene> ScriptEngine::GetSceneContext() { return s_SceneContext; }

    MonoObject* ScriptEngine::Construct(const String& fullName, bool callConstructor, void** parameters)
    {
        String namespaceName;
        String className;
        String parameterList;

        if (fullName.find(".") != String::npos)
        {
            namespaceName = fullName.substr(0, fullName.find_first_of('.'));
            className = fullName.substr(fullName.find_first_of('.') + 1, (fullName.find_first_of(':') - fullName.find_first_of('.')) - 1);

        }

        if (fullName.find(":") != String::npos)
        {
            parameterList = fullName.substr(fullName.find_first_of(':'));
        }

        MonoClass* klass = mono_class_from_name(s_CoreAssemblyImage, namespaceName.c_str(), className.c_str());
        MonoObject* obj = mono_object_new(mono_domain_get(), klass);

        if (callConstructor)
        {
            MonoMethodDesc* desc = mono_method_desc_new(parameterList.c_str(), NULL);
            MonoMethod* constructor = mono_method_desc_search_in_class(desc, klass);
            MonoObject* exception = nullptr;
            mono_runtime_invoke(constructor, obj, parameters, &exception);
        }

        return obj;
    }

    MonoClass* ScriptEngine::GetCoreClass(const String& fullName)
    {
        if (s_Classes.find(fullName) != s_Classes.end())
            return s_Classes[fullName];

        String namespaceName = "";
        String className;

        if (fullName.find('.') != String::npos)
        {
            namespaceName = fullName.substr(0, fullName.find_last_of('.'));
            className = fullName.substr(fullName.find_last_of('.') + 1);
        }
        else
        {
            className = fullName;
        }

        MonoClass* monoClass = mono_class_from_name(s_CoreAssemblyImage, namespaceName.c_str(), className.c_str());
        if (!monoClass) SPK_CORE_LOG_ERROR("mono_class_from_name failed!");
        s_Classes[fullName] = monoClass;
        return monoClass;
    }

    bool ScriptEngine::IsEntityModuleValid(Entity entity)
    {
        return entity.HasComponent<ScriptComponent>() && ModuleExists(entity.GetComponent<ScriptComponent>().ModuleName);
    }

    void ScriptEngine::OnScriptComponentDestroyed(UUID sceneID, UUID entityID)
    {
        SPK_CORE_ASSERT(s_EntityInstanceMap.find(sceneID) != s_EntityInstanceMap.end(), "Entity not found!");
        auto& entityMap = s_EntityInstanceMap.at(sceneID);
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity not found in EntityInstanceMap!");
        entityMap.erase(entityID);
    }

    void ScriptEngine::LoadSpikeRuntimeAssembly(const String& path)
    {
        MonoDomain* domain = nullptr;
        bool cleanup = false;
        if (s_MonoDomain)
        {
            domain = mono_domain_create_appdomain("Spike Runtime", nullptr);
            mono_domain_set(domain, false);

            cleanup = true;
        }

        s_CoreAssembly = LoadAssembly("Spike-Editor/assets/scripts/ScriptEngine.dll");
        s_CoreAssemblyImage = GetAssemblyImage(s_CoreAssembly);

        auto appAssembly = LoadAssembly(path);
        auto appAssemblyImage = GetAssemblyImage(appAssembly);
        ScriptRegistry::RegisterAll();

        if (cleanup)
        {
            mono_domain_unload(s_MonoDomain);
            s_MonoDomain = domain;
        }

        s_AppAssembly = appAssembly;
        s_AppAssemblyImage = appAssemblyImage;
    }

    void ScriptEngine::ReloadAssembly(const String& path)
    {
        LoadSpikeRuntimeAssembly(path);
        if (s_EntityInstanceMap.size())
        {
            Ref<Scene> scene = ScriptEngine::GetSceneContext();
            SPK_CORE_ASSERT(scene, "No active scene!");
            if (s_EntityInstanceMap.find(scene->GetUUID()) != s_EntityInstanceMap.end())
            {
                auto& entityMap = s_EntityInstanceMap.at(scene->GetUUID());
                for (auto& [entityID, entityInstanceData] : entityMap)
                {
                    const auto& entityMap = scene->GetEntityMap();
                    SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
                    InitScriptEntity(entityMap.at(entityID));
                }
            }
        }
    }

    bool ScriptEngine::ModuleExists(const String& moduleName)
    {
        String NamespaceName, ClassName;
        if (moduleName.find('.') != String::npos)
        {
            NamespaceName = moduleName.substr(0, moduleName.find_last_of('.'));
            ClassName = moduleName.substr(moduleName.find_last_of('.') + 1);
        }
        else
            ClassName = moduleName;

        MonoClass* monoClass = mono_class_from_name(s_AppAssemblyImage, NamespaceName.c_str(), ClassName.c_str());
        return monoClass != nullptr;
    }

    /* [Spike] The Creation on the script, instantiating it and Linking it! [Spike] */
    void ScriptEngine::InitScriptEntity(Entity entity)
    {
        Scene* scene = entity.m_Scene;
        UUID id = entity.GetComponent<IDComponent>().ID;
        auto& moduleName = entity.GetComponent<ScriptComponent>().ModuleName;

        if (moduleName == "SpikeNull") return;
        if (!ModuleExists(moduleName)) return;

        EntityScriptClass& scriptClass = s_EntityClassMap[moduleName];
        scriptClass.FullName = moduleName;
        if (moduleName.find('.') != String::npos)
        {
            scriptClass.NamespaceName = moduleName.substr(0, moduleName.find_last_of('.'));
            scriptClass.ClassName = moduleName.substr(moduleName.find_last_of('.') + 1);
        }
        else
        {
            scriptClass.ClassName = moduleName;
        }

        scriptClass.Class = GetClass(s_AppAssemblyImage, scriptClass);
        scriptClass.InitClassMethods(s_AppAssemblyImage);

        EntityInstanceData& entityInstanceData = s_EntityInstanceMap[scene->GetUUID()][id];
        EntityInstance& entityInstance = entityInstanceData.Instance;
        entityInstance.ScriptClass = &scriptClass;
        ScriptModuleFieldMap& moduleFieldMap = entityInstanceData.ModuleFieldMap;
        auto& fieldMap = moduleFieldMap[moduleName];

        std::unordered_map<String, PublicField> oldFields;
        oldFields.reserve(fieldMap.size());
        for (auto& [fieldName, field] : fieldMap)
            oldFields.emplace(fieldName, std::move(field));
        fieldMap.clear();

        /* [Spike] Retrieve the public fields [Spike] */
        {
            MonoClassField* iter;
            void* ptr = 0;
            while ((iter = mono_class_get_fields(scriptClass.Class, &ptr)) != NULL)
            {
                const char* name = mono_field_get_name(iter);
                uint32_t flags = mono_field_get_flags(iter);
                if ((flags & MONO_FIELD_ATTR_PUBLIC) == 0)
                    continue;

                MonoType* fieldType = mono_field_get_type(iter);
                FieldType spikeFieldType = MonoTypeToSpikeFieldType(fieldType);

                /* [Spike] TODO: Attributes [Spike] */
                MonoCustomAttrInfo* attr = mono_custom_attrs_from_field(scriptClass.Class, iter);

                if (oldFields.find(name) != oldFields.end())
                {
                    fieldMap.emplace(name, std::move(oldFields.at(name)));
                }
                else
                {
                    PublicField field = { name, spikeFieldType };
                    field.m_EntityInstance = &entityInstance;
                    field.m_MonoClassField = iter;
                    fieldMap.emplace(name, std::move(field));
                }
            }
        }
    }

    void ScriptEngine::ShutdownScriptEntity(Entity entity, const String& moduleName)
    {
        EntityInstanceData& entityInstanceData = GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID());
        ScriptModuleFieldMap& moduleFieldMap = entityInstanceData.ModuleFieldMap;
        if (moduleFieldMap.find(moduleName) != moduleFieldMap.end())
            moduleFieldMap.erase(moduleName);
    }

    void ScriptEngine::OnSceneDestruct(UUID sceneID)
    {
        if (s_EntityInstanceMap.find(sceneID) != s_EntityInstanceMap.end())
        {
            s_EntityInstanceMap.at(sceneID).clear();
            s_EntityInstanceMap.erase(sceneID);
        }
    }

    /* [Spike] This is the base class from which all the Scripts will derive 🌵 [Spike] */
    void ScriptEngine::InstantiateEntityClass(Entity entity)
    {
        Scene* scene = entity.m_Scene;
        UUID id = entity.GetComponent<IDComponent>().ID;
        auto& moduleName = entity.GetComponent<ScriptComponent>().ModuleName;

        EntityInstanceData& entityInstanceData = GetEntityInstanceData(scene->GetUUID(), id);
        EntityInstance& entityInstance = entityInstanceData.Instance;
        SPK_CORE_ASSERT(entityInstance.ScriptClass, "Invalid ScriptClass");
        entityInstance.Handle = Instantiate(*entityInstance.ScriptClass);

        void* param[] = { &id };
        CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->Constructor, param);

        ScriptModuleFieldMap& moduleFieldMap = entityInstanceData.ModuleFieldMap;

        //Copy all the stored value to runtime
        if (moduleFieldMap.find(moduleName) != moduleFieldMap.end())
        {
            auto& publicFields = moduleFieldMap.at(moduleName);
            for (auto& [name, field] : publicFields)
                field.CopyStoredValueToRuntime();
        }
        OnStartEntity(entity);
    }

    EntityInstanceMap& ScriptEngine::GetEntityInstanceMap()
    {
        return s_EntityInstanceMap;
    }

    EntityInstanceData& ScriptEngine::GetEntityInstanceData(UUID sceneID, UUID entityID)
    {
        /* [Spike] TODO: why it crashes on removing script component lol 😅 [Spike] */
        SPK_CORE_ASSERT(s_EntityInstanceMap.find(sceneID) != s_EntityInstanceMap.end(), "Invalid scene ID!");
        auto& entityIDMap = s_EntityInstanceMap.at(sceneID);
        SPK_CORE_ASSERT(entityIDMap.find(entityID) != entityIDMap.end(), "Invalid entity ID!");
        return entityIDMap.at(entityID);
    }

    MonoObject* EntityInstance::GetInstance()
    {
        SPK_CORE_ASSERT(Handle, "Entity has not been instantiated!");
        return mono_gchandle_get_target(Handle);
    }

    void ScriptEngine::CopyEntityScriptData(UUID dst, UUID src)
    {
        SPK_CORE_ASSERT(s_EntityInstanceMap.find(dst) != s_EntityInstanceMap.end(), "Entity already exists in destination!");
        SPK_CORE_ASSERT(s_EntityInstanceMap.find(src) != s_EntityInstanceMap.end(), "Entity already exists in source!");

        auto& dstEntityMap = s_EntityInstanceMap.at(dst);
        auto& srcEntityMap = s_EntityInstanceMap.at(src);

        for (auto& [entityID, entityInstanceData] : srcEntityMap)
        {
            for (auto& [moduleName, srcFieldMap] : srcEntityMap[entityID].ModuleFieldMap)
            {
                auto& dstModuleFieldMap = dstEntityMap[entityID].ModuleFieldMap;
                for (auto& [fieldName, field] : srcFieldMap)
                {
                    SPK_CORE_ASSERT(dstModuleFieldMap.find(moduleName) != dstModuleFieldMap.end(), "Module already exixts!");
                    auto& fieldMap = dstModuleFieldMap.at(moduleName);
                    SPK_CORE_ASSERT(fieldMap.find(fieldName) != fieldMap.end(), "Field already exixts!");
                    fieldMap.at(fieldName).SetStoredValueRaw(field.m_StoredValueBuffer);
                }
            }
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    static uint32_t GetFieldSize(FieldType type)
    {
        switch (type)
        {
            case FieldType::Float:       return 4;
            case FieldType::Int:         return 4;
            case FieldType::UnsignedInt: return 4;
            // case FieldType::String:   return 16; // TODO
            case FieldType::Vec2:        return 4 * 2;
            case FieldType::Vec3:        return 4 * 3;
            case FieldType::Vec4:        return 4 * 4;
        }
        SPK_CORE_ASSERT(false, "Unknown field type!");
        return 0;
    }

    PublicField::PublicField(const String& name, FieldType type)
        :Name(name), Type(type)
    {
        m_StoredValueBuffer = AllocateBuffer(type);
    }

    PublicField::PublicField(PublicField && other)
    {
        Name = std::move(other.Name);
        Type = other.Type;
        m_EntityInstance = other.m_EntityInstance;
        m_MonoClassField = other.m_MonoClassField;
        m_StoredValueBuffer = other.m_StoredValueBuffer;

        other.m_EntityInstance = nullptr;
        other.m_MonoClassField = nullptr;
        other.m_StoredValueBuffer = nullptr;
    }

    PublicField::~PublicField()
    {
        delete[] m_StoredValueBuffer;
    }

    void PublicField::CopyStoredValueToRuntime()
    {
        SPK_CORE_ASSERT(m_EntityInstance->GetInstance(), "Instance is null!");
        mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, m_StoredValueBuffer);
    }

    bool PublicField::IsRuntimeAvailable() const
    {
        return m_EntityInstance->Handle != 0;
    }

    void PublicField::SetStoredValueRaw(void* src)
    {
        uint32_t size = GetFieldSize(Type);
        memcpy(m_StoredValueBuffer, src, size);
    }

    uint8_t* PublicField::AllocateBuffer(FieldType type)
    {
        uint32_t size = GetFieldSize(type);
        uint8_t* buffer = new uint8_t[size];
        memset(buffer, 0, size);
        return buffer;
    }

    void PublicField::SetStoredValue_Internal(void* value) const
    {
        uint32_t size = GetFieldSize(Type);
        memcpy(m_StoredValueBuffer, value, size);
    }

    void PublicField::GetStoredValue_Internal(void* outValue) const
    {
        uint32_t size = GetFieldSize(Type);
        memcpy(outValue, m_StoredValueBuffer, size);
    }

    void PublicField::SetRuntimeValue_Internal(void* value) const
    {
        SPK_CORE_ASSERT(m_EntityInstance->GetInstance(), "Instance is null!");
        mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, value);
    }

    void PublicField::GetRuntimeValue_Internal(void* outValue) const
    {
        SPK_CORE_ASSERT(m_EntityInstance->GetInstance(), "Instance is null!");
        mono_field_get_value(m_EntityInstance->GetInstance(), m_MonoClassField, outValue);
    }
}
