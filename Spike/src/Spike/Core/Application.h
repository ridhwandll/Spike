//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#pragma once
#include "Base.h"
#include "Window.h"
#include "Spike/Core/LayerStack.h"
#include "Spike/Events/Event.h"
#include "Spike/Events/ApplicationEvent.h"
#include "Spike/ImGui/ImGuiLayer.h"
#include "Spike/Core/Timestep.h"

namespace Spike
{
    class Application
    {
    public:
        Application(const String& name = "Spike Application");
        virtual ~Application();
        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void SetAppIcon(const String& imagePath);
        const char* GetScriptEngineAppAssemblyPath();

        static const char* GetPlatformName();
        static const char* GetConfigurationName();
        static const char* CurrentGraphicsAPI();

        Window& GetWindow() { return *m_Window; }
        void Close();

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
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
        const char* m_ScriptEngineAppAssemblyPath;
    private:
        static Application* s_Instance;
    };
    // To be defined in CLIENT
    Application* CreateApplication();

}