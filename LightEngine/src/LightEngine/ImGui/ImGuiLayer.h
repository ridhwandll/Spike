#pragma once

#include "LightEngine/Core/Layer.h"
#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/Events/KeyEvent.h"
#include "LightEngine/Events/MouseEvent.h"

namespace LightEngine
{
    class  ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();
        void BlockEvents(bool block) { m_BlockEvents = block; }
        void SetDarkThemeColors();
    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };

}