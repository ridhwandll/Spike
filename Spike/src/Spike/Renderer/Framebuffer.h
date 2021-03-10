//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Ref.h"
#include <glm/glm.hpp>

namespace Spike
{
    enum class FormatCode
    {
        R32G32B32A32_FLOAT = 2,
        R8G8B8A8_UNORM = 28,
        D24_UNORM_S8_UINT = 45
    };

    enum class BindFlag
    {
        VERTEX_BUFFER = 0x1L,
        INDEX_BUFFER = 0x2L,
        CONSTANT_BUFFER = 0x4L,
        SHADER_RESOURCE = 0x8L,
        STREAM_OUTPUT = 0x10L,
        RENDER_TARGET = 0x20L,
        DEPTH_STENCIL = 0x40L,
        UNORDERED_ACCESS = 0x80L,
        DECODER = 0x200L,
        VIDEO_ENCODER = 0x400L
    };
    inline BindFlag operator|(BindFlag a, BindFlag b) { return (BindFlag)((uint32_t)a | (uint32_t)b); };

    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        uint32_t Samples = 1;

        struct BufferDesc
        {
            BufferDesc() = default;
            BufferDesc(FormatCode format, BindFlag bindFlags)
                :BindFlags(bindFlags), Format(format) {}

            FormatCode Format;
            BindFlag BindFlags;
        };

        Vector<BufferDesc> BufferDescriptions;
        bool SwapChainTarget = false;
    };

    class Framebuffer : public RefCounted
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual FramebufferSpecification& GetSpecification() = 0;
        virtual void Clear(const glm::vec4& clearColor) = 0;
        virtual RendererID GetColorViewID() = 0;
        virtual RendererID GetSwapChainTarget() = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}