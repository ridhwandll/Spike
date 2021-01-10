/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : OpenGLRendererAPI
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
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Spike
{
    static void OpenGLLogErrorMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            SPK_CORE_LOG_ERROR("[OpenGL Debug HIGH] {0}", message);
            SPK_CORE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            SPK_CORE_LOG_WARN("[OpenGL Debug MEDIUM] {0}", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            SPK_CORE_LOG_INFO("[OpenGL Debug LOW] {0}", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
             SPK_CORE_LOG_TRACE("[OpenGL Debug NOTIFICATION] {0}", message);
            break;
        }
    }

    void OpenGLRendererAPI::Init()
    {
        LE_PROFILE_FUNCTION();

        glDebugMessageCallback(OpenGLLogErrorMessage, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        GLenum error = glGetError();
        while (error != GL_NO_ERROR)
        {
            SPK_CORE_LOG_ERROR("OpenGL Error {0}", error);
            error = glGetError();
        }
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}