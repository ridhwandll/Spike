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
    struct EntityScriptClass;
    struct EntityInstance
    {
        EntityScriptClass* ScriptClass;
        uint32_t Handle = 0;
        /* [Spike] This class represents a single Entity [Spike] */
        Scene* SceneInstance = nullptr;
        MonoObject* GetInstance();
    };

    /* [Spike] The whole Script Engine, initialized at Application.cpp [Spike] */
    class ScriptEngine
    {
        /* [Spike] Initialize the Script Engine [Spike] */
        static void Init(const std::string& assemblyPath);
        static void Shutdown();
        static void Test(); //TODO: Remove!

        /* [Spike] Script Engine must have a scene to work on, we set the scene context by this [Spike] */
        void SetSceneContext(Ref<Scene> scene);
        Ref<Scene> GetSceneContext();
    };
}