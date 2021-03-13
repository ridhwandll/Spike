//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "OpenGLTexture.h"
#include "Spike/Core/Vault.h"
#include <filesystem>
#include <stb_image.h>

namespace Spike
{
    OpenGLTexture2D::OpenGLTexture2D(Uint width, Uint height)
        :m_Width(width), m_Height(height)
    {
        Uint rendererID;
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

    OpenGLTexture2D::OpenGLTexture2D(const String& path, bool flipped)
        : m_FilePath(path), m_Name(Vault::GetNameWithExtension(path))
    {
        LoadTexture(flipped);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
        glDeleteTextures(1, &rendererID);
    }

    void OpenGLTexture2D::Bind(Uint slot, ShaderDomain domain) const
    {
        GLenum textureUnit = GL_TEXTURE0 + slot;
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, (GLuint)m_RendererID);
    }

    void OpenGLTexture2D::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::Reload(bool flip)
    {
        if (m_RendererID)
        {
            Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
            glDeleteTextures(1, &rendererID);
        }
        if (!m_FilePath.empty())
            LoadTexture(flip);
    }

    void OpenGLTexture2D::LoadTexture(bool flip)
    {
        int width, height, channels;

        stbi_set_flip_vertically_on_load(flip);
        stbi_uc* data = stbi_load(m_FilePath.c_str(), &width, &height, &channels, 0);
        if (!data)
            SPK_CORE_LOG_ERROR("Failed to load image from filepath '%s'!", m_FilePath.c_str());

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

        Uint rendererID;
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

    void OpenGLTexture2D::SetData(void* data, Uint size)
    {
        Uint bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        SPK_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glBindTexture(GL_TEXTURE_2D, (GLuint)m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    /*
        Texture Cube
    */

    OpenGLTextureCube::OpenGLTextureCube(const String& folderPath)
    {
        Vector<String> paths = Vault::GetAllFilePathsFromParentPath(folderPath);
        m_FilePath = folderPath;
        m_Name = Vault::GetNameWithoutExtension(folderPath);

        for (uint8_t i = 0; i < 6; i++)
            m_Faces.push_back(paths[i].c_str());

        std::sort(m_Faces.begin(), m_Faces.end());

        for (uint8_t i = 0; i < 6; i++)
            SPK_CORE_LOG_INFO(m_Faces[i].c_str());

        LoadTextureCube(false);
    }

    OpenGLTextureCube::~OpenGLTextureCube()
    {
        Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
        glDeleteTextures(1, &rendererID);
    }

    void OpenGLTextureCube::Bind(Uint slot, ShaderDomain domain) const
    {
        Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
        glBindTextureUnit(slot, rendererID);
    }

    void OpenGLTextureCube::Reload(bool flip) { LoadTextureCube(flip); }

    void OpenGLTextureCube::Unbind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void OpenGLTextureCube::LoadTextureCube(bool flip)
    {
        Uint rendererID;
        glGenTextures(1, &rendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);

        stbi_set_flip_vertically_on_load(flip);
        for (uint8_t i = 0; i < m_Faces.size(); i++)
            SetTexture(i, m_Faces[i]);

        m_Loaded = true;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        m_RendererID = (RendererID)rendererID;
    }

    void OpenGLTextureCube::SetTexture(Uint side, const String& file)
    {
        int width, height, channels;
        stbi_uc* data = stbi_load(file.c_str(), &width, &height, &channels, 0);
        m_Width = width; m_Height = height;

        Uint internalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;
        Uint dataFormat = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;
        if (data)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

        free(data);
    }
}
