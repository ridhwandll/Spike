/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptRegistry
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

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "ScriptRegistry.h"
#include "ScriptCalls.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

/* The Purpose of this file is to register what response should be given
 * from C++ side when the user makes a C# call. Like the Spike::Scripting::Spike_LogInfo
 * is called when the user writes Spike.Console::LogInfo in their C# code
 * and executes this Spike::Scripting::Spike_LogInfo function under the hood */

namespace Spike
{
    void ScriptRegistry::RegisterAll()
    {
        mono_add_internal_call("Spike.Console::LogInfo_Native", Spike::Scripting::Spike_LogInfo);
        mono_add_internal_call("Spike.Console::LogWarn_Native", Spike::Scripting::Spike_LogWarn);
        mono_add_internal_call("Spike.Console::LogDebug_Native", Spike::Scripting::Spike_LogDebug);
        mono_add_internal_call("Spike.Console::LogError_Native", Spike::Scripting::Spike_LogError);
        mono_add_internal_call("Spike.Console::LogFatal_Native", Spike::Scripting::Spike_LogCritical);
    }
}
