/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : OpenGLConstantBuffer
File Type      : cpp
File created on: 2021/02/21
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "OpenGLConstantBuffer.h"
#include <glad/glad.h>

namespace Spike
{
    static GLenum SpikeDataUsageToOpenGLDataUsage(DataUsage usage)
    {
        switch (usage)
        {
            case DataUsage::DEFAULT: return GL_STATIC_DRAW;
            case DataUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
        }
        SPK_CORE_LOG_ERROR("Unknown Spike DataUsage! Returning GL_DYNAMIC_DRAW...");
        return GL_DYNAMIC_DRAW;
    }

    OpenGLConstantBuffer::OpenGLConstantBuffer(const Ref<Shader>& shader, const String& name, void* data, const uint32_t size, const uint32_t bindSlot, ShaderDomain shaderDomain, DataUsage usage)
        :m_Name(name), m_Data(data), m_Size(size), m_BindSlot(bindSlot), m_ShaderDomain(shaderDomain), m_DataUsage(usage)
    {
        uint32_t index = glGetUniformBlockIndex((GLuint)shader->GetRendererID(), name.c_str());
        glUniformBlockBinding((GLuint)shader->GetRendererID(), index, bindSlot);
        uint32_t rendererID;

        glGenBuffers(1, &rendererID);
        glBindBuffer(GL_UNIFORM_BUFFER, rendererID);
        glBufferData(GL_UNIFORM_BUFFER, size, data, SpikeDataUsageToOpenGLDataUsage(usage));
        glBindBuffer(GL_UNIFORM_BUFFER, rendererID);
        glBindBufferBase(GL_UNIFORM_BUFFER, m_BindSlot, rendererID);
        m_RendererID = (RendererID)rendererID;
    }

    void OpenGLConstantBuffer::Bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, (uint32_t)m_RendererID);
    }

    void OpenGLConstantBuffer::SetData(void* data)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, m_Size, data);
    }
}