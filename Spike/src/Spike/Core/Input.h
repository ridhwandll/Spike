//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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