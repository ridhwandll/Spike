//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "OpenGLTexture.h"
#include "Spike/Core/Vault.h"
#include <stb_image.h>

namespace Spike
{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        :m_Width(width), m_Height(height)
    {
        uint32_t rendererID;
        m_Name = "[Spike] Internal Texture [Spike]";
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
        : m_Path(path), m_Name(Vault::GetNameWithExtension(path))
    {
        int width, height, channels;

        stbi_set_flip_vertically_on_load(false);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16); //TODO: Render Caps

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0); //Always remember to Unbind the Texture
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
