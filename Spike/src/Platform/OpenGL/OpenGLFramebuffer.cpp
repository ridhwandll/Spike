//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "OpenGLFramebuffer.h"
#include "glad/glad.h"

namespace Spike
{
    static const uint32_t s_MaxFramebufferSize = 8192;

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
        :m_Specification(spec)
    {
        Invalidate();
    }

    void OpenGLFramebuffer::Invalidate()
    {
        uint32_t rendererID;
        glGenFramebuffers(1, &rendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

        CreateColorView();
        CreateDepthView();

        SPK_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind the Framebuffer
        m_RendererID = (RendererID)rendererID;
    }

    void OpenGLFramebuffer::CreateColorView()
    {
        glGenTextures(1, &m_ColorAttachmentID);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0); //Attach the texture
    }

    void OpenGLFramebuffer::CreateDepthView()
    {
        glGenTextures(1, &m_DepthAttachmentID);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachmentID, 0); //Attach the texture
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            SPK_CORE_LOG_WARN("Attempted to resize framebuffer to (Width: %i, Height: %i)", width, height);
            return;
        }
        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }

    void OpenGLFramebuffer::Clear(const glm::vec4& clearColor)
    {
        uint32_t rendererID = reinterpret_cast<uint32_t>(m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
        glClearColor(clearColor.x, clearColor.y, clearColor.y, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    RendererID OpenGLFramebuffer::GetColorViewID()
    {
        return (void*)m_ColorAttachmentID;
    }

    RendererID OpenGLFramebuffer::GetSwapChainTarget()
    {
        return (RendererID)nullptr;
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        uint32_t rendererID = reinterpret_cast<uint32_t>(m_RendererID);
        glDeleteFramebuffers(1, &rendererID);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Clean()
    {
        m_ColorAttachmentID = 0;
        m_DepthAttachmentID = 0;
    }
}