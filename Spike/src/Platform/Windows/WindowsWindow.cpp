/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : WindowsWindow
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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/

#include "spkpch.h"
#include "WindowsWindow.h"
#include "Spike/Core/Input.h"
#include "Spike/Events/ApplicationEvent.h"
#include "Spike/Events/MouseEvent.h"
#include "Spike/Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

// GLFW is temporary. In Windows it will be replaced by WIN32 API
namespace Spike
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        SPK_CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        LE_PROFILE_FUNCTION();
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        LE_PROFILE_FUNCTION();
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        LE_PROFILE_FUNCTION();
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (!s_GLFWInitialized)
        {
            LE_PROFILE_SCOPE("glfwInit");
            // TODO: glfwTerminate on system shutdown
            int success = glfwInit();
            SPK_CORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }
        {
            LE_PROFILE_SCOPE("glfwCreateWindow");
            m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        }

        m_Context = CreateScope<OpenGLContext>(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        //Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent e(width, height);
                data.EventCallback(e);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent e;
                data.EventCallback(e);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent e(key, 0);
                        data.EventCallback(e);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent e(key);
                        data.EventCallback(e);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        KeyPressedEvent e(key, 1);
                        data.EventCallback(e);
                        break;
                    }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent e(static_cast<KeyCode>(keycode));
                data.EventCallback(e);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent e(button);
                        data.EventCallback(e);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent e(button);
                        data.EventCallback(e);
                        break;
                    }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent e((float)xOffset, (float)yOffset);
                data.EventCallback(e);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMovedEvent e((float)xPos, (float)yPos);
                data.EventCallback(e);
            });
    }

    void WindowsWindow::Shutdown()
    {
        LE_PROFILE_FUNCTION();
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        LE_PROFILE_FUNCTION();
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        LE_PROFILE_FUNCTION();
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

}
