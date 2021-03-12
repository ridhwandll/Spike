//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "OpenGLVertexBuffer.h"
#include <glad/glad.h>

namespace Spike
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(Uint size, VertexBufferLayout layout)
        :m_Layout(layout)
    {
        Uint rendererID;

        glGenBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        m_RendererID = (RendererID)rendererID;
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, Uint size, VertexBufferLayout layout)
        :m_Layout(layout)
    {
        Uint rendererID;
        glGenBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        m_RendererID = (RendererID)rendererID;
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
        glDeleteBuffers(1, &rendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, (GLuint)m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void* data, Uint size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, (GLuint)m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}