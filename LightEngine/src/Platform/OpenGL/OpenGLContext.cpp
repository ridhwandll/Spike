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
        LE_PROFILE_FUNCTION();
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LE_CORE_ASSERT(status, "Failed to initialize Glad!");
        LE_CORE_LOG_INFO("--OpenGl INFO--");
        LE_CORE_LOG_INFO("Vendor  : {0}", glGetString(GL_VENDOR));
        LE_CORE_LOG_INFO("Renderer: {0}", glGetString(GL_RENDERER));
        LE_CORE_LOG_INFO("Version : {0}", glGetString(GL_VERSION));
        LE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "LightEngine requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        LE_PROFILE_FUNCTION();
        glfwSwapBuffers(m_WindowHandle);
    }

}
