/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Material
File Type      : h
File created on: 2021/03/04
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/Shader.h"
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/ConstantBuffer.h"

namespace Spike
{
    struct MaterialCbuffer
    {
        glm::vec3 Color;
        int AlbedoTexToggle;
    };

    class Material : public RefCounted
    {
    public:
        Material() = default;
        Material(const Ref<Shader>& shader);
        ~Material();

        void Bind(uint32_t index);

        Ref<Shader>& GetShader() { return m_Shader; }
        Vector<Ref<Texture2D>>& GetTextures() { return m_Textures; }
        void PushTexture(const Ref<Texture2D>& tex, uint32_t slot = 0);
        void SetColor(const glm::vec3& color) { m_Color = color; }
        void SetAlbedoTexToggle(int value);

        static Ref<Material> Material::Create(const Ref<Shader>& shader);
    private:
        Ref<Shader> m_Shader;
        Vector<Ref<Texture2D>> m_Textures;
        glm::vec3 m_Color;
        int m_AlbedoTexToggleValue;

        MaterialCbuffer m_CBufferData;
        Ref<ConstantBuffer> m_MainCBuffer;
    };
}