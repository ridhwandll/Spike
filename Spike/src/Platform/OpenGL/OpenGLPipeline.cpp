//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Platform/OpenGL/OpenGLPipeline.h"
#include <glad/glad.h>

namespace Spike
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:   return GL_FLOAT;
            case ShaderDataType::Float2:  return GL_FLOAT;
            case ShaderDataType::Float3:  return GL_FLOAT;
            case ShaderDataType::Float4:  return GL_FLOAT;
            case ShaderDataType::Mat3:    return GL_FLOAT;
            case ShaderDataType::Mat4:    return GL_FLOAT;
            case ShaderDataType::Int:     return GL_INT;
            case ShaderDataType::Int2:    return GL_INT;
            case ShaderDataType::Int3:    return GL_INT;
            case ShaderDataType::Int4:    return GL_INT;
            case ShaderDataType::Bool:    return GL_BOOL;
        }
        SPK_INTERNAL_ASSERT("Unknown ShaderDataType!");
        return 0;
    }

    OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& spec)
        :m_Specification(spec)
    {
        SPK_CORE_ASSERT(m_Specification.VertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        Uint rendererID;
        glGenVertexArrays(1, &rendererID);
        glBindVertexArray(rendererID);

        m_Specification.VertexBuffer->Bind();

        const auto& layout = m_Specification.VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribPointer(m_VertexBufferIndex,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                uint8_t count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                        count,
                        ShaderDataTypeToOpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (const void*)(element.Offset + sizeof(float) * count * i));
                    glVertexAttribDivisor(m_VertexBufferIndex, 1);
                    m_VertexBufferIndex++;
                }
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
            {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribIPointer(m_VertexBufferIndex,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    layout.GetStride(),
                    (const void*)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            default:
                SPK_INTERNAL_ASSERT("Unknown ShaderDataType!");
            }
        }
        glBindVertexArray(rendererID);
        m_Specification.IndexBuffer->Bind();
        m_RendererID = (RendererID)rendererID;
    }

    OpenGLPipeline::~OpenGLPipeline()
    {
        Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
        glDeleteVertexArrays(1, &rendererID);
    }

    void OpenGLPipeline::Bind() const
    {
        glBindVertexArray((GLuint)m_RendererID);
    }

    void OpenGLPipeline::BindSpecificationObjects() const
    {
        m_Specification.VertexBuffer->Bind();
        m_Specification.IndexBuffer->Bind();
        m_Specification.Shader->Bind();
    }

    void OpenGLPipeline::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLPipeline::SetPrimitiveTopology(PrimitiveTopology topology)
    {
        //TODO
    }
}