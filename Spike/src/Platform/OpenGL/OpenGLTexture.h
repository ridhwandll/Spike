//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Texture.h"
#include "Spike/Core/Buffer.h"
#include <glad/glad.h>

namespace Spike
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(Uint width, Uint height);
        OpenGLTexture2D(const String& path, bool flipped = false);
        virtual ~OpenGLTexture2D();
        virtual String const GetName() const override { return m_Name; }
        virtual Uint GetWidth() const override { return m_Width; }
        virtual Uint GetHeight() const override { return m_Height; }
        virtual RendererID GetRendererID() const override { return (RendererID)m_RendererID; };
        virtual String GetFilepath() const override { return m_FilePath; }
        void SetData(void* data, Uint size) override;
        virtual void ActivateSlot(Uint slot) override;
        virtual void Bind(Uint slot = 0, ShaderDomain domain = ShaderDomain::PIXEL) const override;
        virtual void Unbind() const override;
        virtual bool Loaded() override { return m_Loaded; }
        virtual void Reload(bool flip = false);
        bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }
    private:
        void LoadTexture(bool flip);
    private:
        bool m_Loaded = false;
        String m_FilePath;
        Uint m_Width, m_Height;
        RendererID m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
        String m_Name;
    };
}