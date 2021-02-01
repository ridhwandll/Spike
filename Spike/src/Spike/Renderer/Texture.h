/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Texture
File Type      : h
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
#pragma once
#include "Spike/Core/Ref.h"
#include <string>
#include "Spike/Core/Base.h"

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

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;
        virtual void ActivateSlot(uint32_t slot) = 0;
        virtual bool Loaded() = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind() const = 0;
        virtual bool operator==(const Texture& other) const = 0;

        static uint32_t CalculateMipMapCount(uint32_t width, uint32_t height);
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        static Ref<Texture2D> Create(const String& path);
        static Ref<Texture2D> Create(const String& path, bool flipVertically, bool srgb);
    };

}