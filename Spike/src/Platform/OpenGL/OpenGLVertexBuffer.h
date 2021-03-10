//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/VertexBuffer.h"

namespace Spike
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size, VertexBufferLayout layout);
        OpenGLVertexBuffer(void* vertices, uint32_t size, VertexBufferLayout layout);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual const VertexBufferLayout& GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }
    private:
        RendererID m_RendererID = 0;
        VertexBufferLayout m_Layout;
    };

}