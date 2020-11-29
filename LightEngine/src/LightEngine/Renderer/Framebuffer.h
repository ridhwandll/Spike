#pragma once
#include "glm/glm.hpp"

namespace LightEngine
{
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(const uint32_t width, const uint32_t height) = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual const FramebufferSpecification& GetSpecification() const = 0;
        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}