/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : OpenGLTexture
File Type      : cpp
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "OpenGLTexture.h"
#include "Spike/Core/Vault.h"
#include <stb_image.h>

namespace Spike
{
    static GLenum ConvertToOpenGLTextureFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGB:     return GL_RGB;
            case TextureFormat::RGBA:    return GL_RGBA;
            case TextureFormat::Float16: return GL_RGBA16F;
        }
        SPK_INTERNAL_ASSERT("Unknown texture format!");
        return 0;
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        :m_Width(width), m_Height(height)
    {
        uint32_t rendererID;
        m_Name = "[Spike] Internal Texture";
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;
        glGenTextures(1, &rendererID);
        glBindTexture(GL_TEXTURE_2D, rendererID);

        m_Loaded = true;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        m_RendererID = reinterpret_cast<RendererID>(rendererID);
    }

    OpenGLTexture2D::OpenGLTexture2D(const String& path)
        : m_Path(path)
    {
        m_Name = Vault::GetNameWithoutExtension(path);
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = nullptr;
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data)
            SPK_CORE_LOG_CRITICAL("Failed to load image!");

        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
          internalFormat = GL_RGB8;
          dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        if (!(internalFormat & dataFormat))
        {
            SPK_CORE_LOG_CRITICAL("Texture format not supported!");
            return;
        }

        m_Loaded = true;

        uint32_t rendererID;
        glGenTextures(1, &rendererID);
        glBindTexture(GL_TEXTURE_2D, rendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum type = internalFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        m_RendererID = (RendererID)rendererID;
        free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        uint32_t rendererID = reinterpret_cast<uint32_t>(m_RendererID);
        glDeleteTextures(1, &rendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot, ShaderDomain domain) const
    {
        GLenum textureUnit = GL_TEXTURE0 + slot;
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, (GLuint)m_RendererID);
    }

    void OpenGLTexture2D::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        SPK_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glBindTexture(GL_TEXTURE_2D, (GLuint)m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::ActivateSlot(uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

}
