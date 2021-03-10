//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Base.h"
#include "Spike/Events/Event.h"
#include "Spike/Core/Timestep.h"

namespace Spike
{
    class  Layer
    {
    public:
        Layer(const String& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& e) {}

        const String& GetName() const { return m_DebugName; }
    protected:
        String m_DebugName;
    };

}