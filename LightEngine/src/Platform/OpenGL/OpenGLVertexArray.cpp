#include "lepch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace LightEngine
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case LightEngine::ShaderDataType::Float:   return GL_FLOAT;
            case LightEngine::ShaderDataType::Float2:  return GL_FLOAT;
            case LightEngine::ShaderDataType::Float3:  return GL_FLOAT;
            case LightEngine::ShaderDataType::Float4:  return GL_FLOAT;
            case LightEngine::ShaderDataType::Mat3:    return GL_FLOAT;
            case LightEngine::ShaderDataType::Mat4:    return GL_FLOAT;
            case LightEngine::ShaderDataType::Int:     return GL_INT;
            case LightEngine::ShaderDataType::Int2:    return GL_INT;
            case LightEngine::ShaderDataType::Int3:    return GL_INT;
            case LightEngine::ShaderDataType::Int4:    return GL_INT;
            case LightEngine::ShaderDataType::Bool:    return GL_BOOL;
        }
        LE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        LE_PROFILE_FUNCTION();
        glGenVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        LE_PROFILE_FUNCTION();
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        LE_PROFILE_FUNCTION();
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        LE_PROFILE_FUNCTION();
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        LE_PROFILE_FUNCTION();
        LE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();
        const auto& layout = vertexBuffer->GetLayout();
        uint32_t index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        LE_PROFILE_FUNCTION();
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}