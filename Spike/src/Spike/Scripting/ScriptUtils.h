/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptUtils
File Type      : h
File created on: 2021/01/30
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

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Scene/Components.h"
#include "Spike/Scripting/ScriptEngine.h"
#include <mono/jit/jit.h>

namespace Spike::Scripting
{
    /* [Spike] Utils begin [Spike] */
    String ConvertMonoStringToCppString(MonoString* message)
    {
        /* [Spike] Any way to make this efficient? we are doing 2 copies here! [Spike] */
        char* ptr = mono_string_to_utf8(message);
        String s{ ptr };
        mono_free(ptr); /* [Spike] Yes, we need to free this with mono_free [Spike] */
        return s;
    }

    MonoString* ConvertCppStringToMonoString(MonoDomain* domain, const std::string& str)
    {
        return mono_string_new(domain, str.c_str());
    }

    EntityMap ValidateSceneAndReturnEntityMap(Ref<Scene>& sceneContext, uint64_t entityID)
    {
        SPK_CORE_ASSERT(sceneContext, "No active scene!");
        const auto& entityMap = sceneContext->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");
        return entityMap;
    }

    template<typename T>
    T& ValidateSceneAndReturnAComponent(Ref<Scene>& sceneContext, uint64_t entityID)
    {
        auto& entityMap = ValidateSceneAndReturnEntityMap(ScriptEngine::GetSceneContext(), entityID);
        Entity entity = entityMap.at(entityID);
        SPK_INTERNAL_ASSERT(entity.HasComponent<T>());
        auto& component = entity.GetComponent<T>();
        return component;
    }
    /* [Spike] Utils end [Spike] */
}
