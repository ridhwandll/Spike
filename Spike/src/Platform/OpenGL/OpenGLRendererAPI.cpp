//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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
            SPK_CORE_LOG_ERROR("[OpenGL Debug HIGH] %s", message);
            SPK_INTERNAL_ASSERT("GL_DEBUG_SEVERITY_HIGH");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            SPK_CORE_LOG_WARN("[OpenGL Debug MEDIUM] %s", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            SPK_CORE_LOG_INFO("[OpenGL Debug LOW] %s", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
             //SPK_CORE_LOG_TRACE("[OpenGL Debug NOTIFICATION] %s", message);
            break;
        }
    }

    void OpenGLRendererAPI::Init()
    {
        glDebugMessageCallback(OpenGLLogErrorMessage, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        auto& caps = RendererAPI::GetCapabilities();

        caps.Vendor = (const char*)glGetString(GL_VENDOR);
        caps.Renderer = (const char*)glGetString(GL_RENDERER);
        caps.Version = (const char*)glGetString(GL_VERSION);

        glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);

        GLenum error = glGetError();
        while (error != GL_NO_ERROR)
        {
            SPK_CORE_LOG_ERROR("OpenGL Error %s", error);
            error = glGetError();
        }
    }

    void OpenGLRendererAPI::SetViewport(Uint x, Uint y, Uint width, Uint height)
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

    void OpenGLRendererAPI::DrawIndexed(const Ref<Pipeline>& pipeline, Uint indexCount)
    {
        Uint count = indexCount ? indexCount : pipeline->GetSpecification().IndexBuffer->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLRendererAPI::DrawIndexedMesh(Uint indexCount, Uint baseIndex, Uint baseVertex)
    {
        glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(sizeof(Uint) * baseIndex), baseVertex);
    }

    void OpenGLRendererAPI::BindBackbuffer() {}

    void OpenGLRendererAPI::BeginWireframe()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void OpenGLRendererAPI::EndWireframe()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}