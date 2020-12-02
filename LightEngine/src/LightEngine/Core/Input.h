#pragma once
#include <glm/glm.hpp>
#include "LightEngine/Core/KeyCodes.h"
#include "LightEngine/Core/MouseCodes.h"

namespace LightEngine
{
    class Input
    {
    public:
        static bool IsKeyPressed(const KeyCode keycode);
        static bool IsMouseButtonPressed(const MouseCode button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

}