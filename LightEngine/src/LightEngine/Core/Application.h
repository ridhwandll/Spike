#pragma once
#include "Base.h"
#include "Window.h"
#include "LightEngine/Core/LayerStack.h"
#include "LightEngine/Events/Event.h"
#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/ImGui/ImGuiLayer.h"
#include "LightEngine/Core/Timestep.h"

namespace LightEngine
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        Window& GetWindow() { return *m_Window; }

        static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
    };
    // To be defined in CLIENT
    Application* CreateApplication();

}