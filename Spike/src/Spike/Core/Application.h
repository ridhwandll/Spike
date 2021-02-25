/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Application
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
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
    private:
        static Application* s_Instance;
    };
    // To be defined in CLIENT
    Application* CreateApplication();

}