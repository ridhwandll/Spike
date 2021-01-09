/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : SubTexture2D
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
#include "Texture.h"
#include <glm/glm.hpp>


namespace Spike
{
    class SubTexture2D : public RefCounted
    {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max);

        const glm::vec2* GetTexCoords() { return m_TexCoords; }
        const Ref<Texture2D> GetTexture() { return m_Texture; }

        static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1, 1});
    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_TexCoords[4];
    };
}