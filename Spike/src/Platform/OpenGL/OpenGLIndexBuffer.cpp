//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "OpenGLIndexBuffer.h"
#include <glad/glad.h>

namespace Spike
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(void* indices, uint32_t count)
        : m_Count(count)
    {
        uint32_t rendererID;
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
        m_RendererID = (RendererID)rendererID;
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        uint32_t rendererID = reinterpret_cast<uint32_t>(m_RendererID);
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
