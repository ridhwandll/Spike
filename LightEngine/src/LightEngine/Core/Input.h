#pragma once
#include "LightEngine/Core/Base.h"
#include "LightEngine/Core/KeyCodes.h"
#include "LightEngine/Core/MouseCodes.h"

namespace LightEngine
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode keycode);
        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

}