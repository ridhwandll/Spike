/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ScriptCalls
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
#include "Spike/Core/Input.h"
#include <glm/glm.hpp>
#include <mono/metadata/object.h>

namespace Spike::Scripting
{
    /* [Spike] Console [Spike] */
    void Spike_Console_LogInfo(MonoString* message);
    void Spike_Console_LogWarn(MonoString* message);
    void Spike_Console_LogDebug(MonoString* message);
    void Spike_Console_LogError(MonoString* message);
    void Spike_Console_LogCritical(MonoString* message);

    /* [Spike] Input [Spike] */
    bool Spike_Input_IsKeyPressed(KeyCode key);
    bool Spike_Input_IsMouseButtonPressed(MouseCode button);
    void Spike_Input_GetMousePosition(glm::vec2* outPosition);
    void Spike_Input_SetCursorMode(MousePointerMode mode);
    MousePointerMode Spike_Input_GetCursorMode();


} 