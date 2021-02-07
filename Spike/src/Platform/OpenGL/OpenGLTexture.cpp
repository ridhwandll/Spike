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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
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
        m_Name = "[Spike] Internal Texture";
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        m_Loaded = true;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const String& path)
        : m_Path(path)
    {
        m_Name = Vault::GetNameWithoutExtension(path);
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = nullptr;
        {
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        m_Resolution = { width, height };
        if (data == nullptr) //We don't want to assert here....the engine should be running
        {
            SPK_CORE_LOG_CRITICAL("Failed to load image!");
        }
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

        if (!(internalFormat & dataFormat)) //We don't want to assert here....the engine should be running
        {
            SPK_CORE_LOG_CRITICAL("Texture format not supported!");
            return;
        }

        m_Loaded = true;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        GLenum type = internalFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(const String& path, bool flipVertically, bool srgb)
        :m_Path(path)
    {
        m_Name = Vault::GetNameWithoutExtension(path);
        int width, height, channels;
        if (flipVertically)
            stbi_set_flip_vertically_on_load(1);
        else
            stbi_set_flip_vertically_on_load(0);

        stbi_uc* data = nullptr;

        if (stbi_is_hdr(path.c_str()))
        {
            SPK_CORE_LOG_INFO("Loading HDR texture {0}, srgb = {1}", path, srgb);
            data = (stbi_uc*)stbi_loadf(path.c_str(), &width, &height, &channels, 0);
            m_IsHDR = true;
            m_Format = TextureFormat::Float16;
        }
        else
        {
            SPK_CORE_LOG_INFO("Loading texture %s, srgb = %s", path.c_str(), srgb ? "true" : "false");
            data = stbi_load(path.c_str(), &width, &height, &channels, srgb ? STBI_rgb : STBI_rgb_alpha);
            SPK_CORE_ASSERT(data, "Could not read image!");
            m_Format = TextureFormat::RGBA;
        }

        if (!data)
            return;

        m_Loaded = true;
        m_Width = width;
        m_Height = height;

        if (srgb)
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            int levels = Texture::CalculateMipMapCount(m_Width, m_Height);
            glTextureStorage2D(m_RendererID, levels, GL_SRGB8, m_Width, m_Height);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateTextureMipmap(m_RendererID);
        }
        else
        {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            GLenum internalFormat = ConvertToOpenGLTextureFormat(m_Format);
            GLenum format = srgb ? GL_SRGB8 : (m_IsHDR ? GL_RGB : ConvertToOpenGLTextureFormat(m_Format));
            GLenum type = internalFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, type, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        GLenum textureUnit = GL_TEXTURE0 + slot;
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void OpenGLTexture2D::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        SPK_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::ActivateSlot(uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

}
