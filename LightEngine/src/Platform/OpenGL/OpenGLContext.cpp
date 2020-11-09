#include "lepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace LightEngine
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        LE_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LE_CORE_ASSERT(status, "Failed to initialize Glad!");
        LE_CORE_LOG_INFO("--OpenGl INFO--");
        LE_CORE_LOG_INFO("Vendor  : {0}", glGetString(GL_VENDOR));
        LE_CORE_LOG_INFO("Renderer: {0}", glGetString(GL_RENDERER));
        LE_CORE_LOG_INFO("Version : {0}", glGetString(GL_VERSION));

    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}
