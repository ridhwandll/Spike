#pragma once
#include "LightEngine/Core.h"
#include "LightEngine/Events/Event.h"
#include "LightEngine/Core/Timestep.h"

namespace LightEngine
{

    class LIGHTENGINE_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

}