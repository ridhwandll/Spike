/*****************************************************************************/
/*                        Lightengine  SourceCode                            */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: None                                                       */
/*                                                                           */
/*   Licensed under the Apache License, Version 2.0 (the "License");         */
/*   you may not use this file except in compliance with the License.        */
/*   You may obtain a copy of the License at                                 */
/*                                                                           */
/*       http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                           */
/*   Unless required by applicable law or agreed to in writing, software     */
/*   distributed under the License is distributed on an "AS IS" BASIS,       */
/*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*   See the License for the specific language governing permissions and     */
/*   limitations under the License.                                          */
/*****************************************************************************/

#include "lepch.h"
#include "WindowsWindow.h"
#include "LightEngine/Core/Input.h"
#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/Events/MouseEvent.h"
#include "LightEngine/Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

// GLFW is temporary. In Windows it will be replaced by WIN32 API
namespace LightEngine
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        LE_CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
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
            LE_CORE_ASSERT(success, "Could not intialize GLFW!");
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
