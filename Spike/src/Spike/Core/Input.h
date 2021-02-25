/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Input
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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include <glm/glm.hpp>
#include "Spike/Core/KeyCodes.h"
#include "Spike/Core/MouseCodes.h"

namespace Spike
{
    enum class MousePointerMode
    {
        Normal = 0,
        Hidden = 1,
        Locked = 2
    };

    class Input
    {
    public:
        static bool IsKeyPressed(const KeyCode keycode);
        static bool IsMouseButtonPressed(const MouseCode button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
        static void SetCursorMode(MousePointerMode mode);
        static MousePointerMode GetCursorMode();
    };

}