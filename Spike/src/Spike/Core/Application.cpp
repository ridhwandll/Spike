/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Application
File Type      : cpp
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
#include "spkpch.h"
#include "Application.h"
#include "Spike/Core/Log.h"
#include "Spike/Renderer/Renderer.h"
#include "Spike/Renderer/Renderer2D.h"
#include "Spike/Core/Input.h"
#include "Spike/Core/Vault.h"
#include "Spike/Utility/FileDialogs.h"
#include "Spike/Scripting/ScriptEngine.h"
#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace Spike
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application(const String& name)
    {
        SPK_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Scope<Window>(Window::Create(WindowProps(name)));
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ScriptEngineAppAssemblyPath = "Spike-Editor/assets/scripts/ExampleApp.dll";
        ScriptEngine::Init(m_ScriptEngineAppAssemblyPath);
        Renderer::Init();
        Renderer2D::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        Renderer::Shutdown();
        Renderer2D::Shutdown();
        ScriptEngine::Shutdown();
        Vault::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::SetAppIcon(const String& imagePath)
    {
        GLFWimage icons[1];
        icons[0].pixels = stbi_load(imagePath.c_str(), &icons[0].width, &icons[0].height, 0, 4);
        glfwSetWindowIcon(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()), 1, icons);
        stbi_image_free(icons[0].pixels);
    }

    const char* Application::GetScriptEngineAppAssemblyPath()
    {
        return m_ScriptEngineAppAssemblyPath;
    }

    const char* Application::GetPlatformName()
    {
        #if defined SPK_PLATFORM_WINDOWS
                return "Windows x64";
        #elif defined SPK_PLATFORM_MACOS
                return "MacOS";
        #elif defined SPK_PLATFORM_LINUX
                return "Linux";
        #endif
                return "Undefined PlatformName";
    }

    const char* Application::GetConfigurationName()
    {
        #if defined SPK_DEBUG
                return "Debug";
        #elif defined SPK_RELEASE
                return "Release";
        #elif defined SPK_DIST
                return "Dist";
        #endif
                return "Undefined ConfigurationName";
    }

    const char* Application::CurrentGraphicsAPI()
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:
                return "None";
            case RendererAPI::API::OpenGL:
                return "OpenGL 4.5";
            case RendererAPI::API::DX11:
                return "DirectX 11";
        }
        return "RendererAPI not found!";
    }

    void Application::Close()
    {
        if (FileDialogs::AMessageBox("Spike", "Are you sure you want to quit?", DialogType::Yes__No, IconType::Question, DefaultButton::No))
            m_Running = false;
        else
            return;
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    //Engines main RUN loop
    void Application::Run()
    {
        while (m_Running)
        {
            // should be Platform::GetTime();
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if(!m_Minimized)
            {
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin();
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        if (FileDialogs::AMessageBox("Spike", "Are you sure you want to quit?", DialogType::Yes__No, IconType::Question, DefaultButton::No))
            m_Running = false;
        else
            return false;
        return false;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetHeight() == 0 || e.GetWidth() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}