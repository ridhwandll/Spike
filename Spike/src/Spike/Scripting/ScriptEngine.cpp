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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>

namespace Spike
{
    static MonoDomain* s_MonoDomain = nullptr;
    static Ref<Scene> s_SceneContext;
    static std::string s_AssemblyPath;
    static MonoAssembly* s_CoreAssembly;
    static MonoAssembly* s_AppAssembly;

    /* Static Helper Functions for the ScriptEngine starts here. Maybe port this section to it's own file? */
    static void InitMono()
    {
        mono_set_assemblies_path("Spike/vendor/mono/lib");
        MonoDomain* domain = mono_jit_init("Spike");
        /* [Spike] Register an app domain [Spike] */
        char* name = (char*)"Spike-Runtime";
        s_MonoDomain = mono_domain_create_appdomain(name, nullptr);
    }

    static void ShutdownMono()
    {
        /* [Spike] must be called if InitMono() is called [Spike] */
        mono_jit_cleanup(s_MonoDomain);
    }

    /* Static Helper Functions for the ScriptEngine ends here */

    void ScriptEngine::Init(const std::string& assemblyPath)
    {
        s_AssemblyPath = assemblyPath;
        InitMono();
    }

    void ScriptEngine::Shutdown()
    {
        //ShutdownMono();
        s_SceneContext = nullptr;
    }

    void ScriptEngine::Test()
    {

    }

    void ScriptEngine::SetSceneContext(Ref<Scene> scene) { s_SceneContext = scene; }
    Ref<Spike::Scene> ScriptEngine::GetSceneContext() { return s_SceneContext; }

}