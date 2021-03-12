//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Ref.h"
#include "Spike/Core/Base.h"
#include "Spike/Renderer/Shader.h"
#include <string>
#include <glm/glm.hpp>

namespace Spike
{
    enum class TextureFormat
    {
        None = 0,
        RGB = 1,
        RGBA = 2,
        Float16 = 3
    };

    enum class TextureWrap
    {
        None = 0,
        Clamp = 1,
        Repeat = 2
    };

    class Texture : public RefCounted
    {
    public:
        virtual ~Texture() = default;

        virtual Uint GetWidth() const = 0;
        virtual Uint GetHeight() const = 0;
        virtual RendererID GetRendererID() const = 0;
        virtual String GetFilepath() const = 0;
        virtual String const GetName() const = 0;
        virtual void SetData(void* data, Uint size) = 0;
        virtual void ActivateSlot(Uint slot) = 0;
        virtual bool Loaded() = 0;
        virtual void Reload(bool flip = false) = 0;
        virtual void Bind(Uint slot = 0, ShaderDomain domain = ShaderDomain::PIXEL) const = 0;
        virtual void Unbind() const = 0;
        virtual bool operator==(const Texture& other) const = 0;

        static Uint CalculateMipMapCount(Uint width, Uint height);
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(Uint width, Uint height);
        static Ref<Texture2D> Create(const String& path, bool flipped = false);
    };

}