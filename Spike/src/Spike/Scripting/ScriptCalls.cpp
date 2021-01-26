/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptCalls
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
#include "ScriptCalls.h"
#include "Panels/ConsolePanel.h"

namespace Spike::Scripting
{
    std::string ConvertMonoString(MonoString* message)
    {
        /* [Spike] Any way to make this efficient? we are doing 2 copies here! [Spike] */
        char* ptr = mono_string_to_utf8(message);
        std::string s { ptr };
        mono_free(ptr);
        return s;
    }

    void Spike_LogInfo(MonoString* message)
    {
        SPK_CORE_LOG_INFO(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_INFO);
    }

    void Spike_LogWarn(MonoString* message)
    {
        SPK_CORE_LOG_WARN(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_WARN);
    }

    void Spike_LogDebug(MonoString* message)
    {
        SPK_CORE_LOG_DEBUG(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_DEBUG);
    }

    void Spike_LogError(MonoString* message)
    {
        SPK_CORE_LOG_ERROR(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_ERROR);
    }

    void Spike_LogCritical(MonoString* message)
    {
        SPK_CORE_LOG_CRITICAL(ConvertMonoString(message));
        Console::Get()->Print(ConvertMonoString(message), Console::LogLevel::LVL_CRITICAL);
    }

}
