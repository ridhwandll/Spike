//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "OpenGLIndexBuffer.h"
#include <glad/glad.h>

namespace Spike
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(void* indices, Uint count)
        : m_Count(count)
    {
        Uint rendererID;
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, sizeof(Uint) * count, indices, GL_STATIC_DRAW);
        m_RendererID = (RendererID)rendererID;
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
        glDeleteBuffers(1, &rendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}
