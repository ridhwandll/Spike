//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once

#include "Spike/Core/Layer.h"
#include "Spike/Events/ApplicationEvent.h"
#include "Spike/Events/KeyEvent.h"
#include "Spike/Events/MouseEvent.h"

namespace Spike
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
    };

}