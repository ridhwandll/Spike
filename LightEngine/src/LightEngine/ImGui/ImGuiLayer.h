#pragma once

#include "LightEngine/Core/Layer.h"
#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/Events/KeyEvent.h"
#include "LightEngine/Events/MouseEvent.h"

namespace LightEngine
{
    class LIGHTENGINE_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
    };

}