//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Framebuffer.h"

namespace Spike
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual FramebufferSpecification& GetSpecification() override { return m_Specification; }
        virtual void Clear(const glm::vec4& clearColor) override;
        virtual RendererID GetColorViewID() override;
        virtual RendererID GetSwapChainTarget() override;
    private:
        void CreateColorView();
        void CreateDepthView();
        void Invalidate();
        void Clean();
    private:
        FramebufferSpecification m_Specification;
        RendererID m_RendererID;
        uint32_t m_ColorAttachmentID;
        uint32_t m_DepthAttachmentID;
    };

}