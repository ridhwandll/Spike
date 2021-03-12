//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#pragma once
#include "Spike/Renderer/Shader.h"
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/ConstantBuffer.h"

namespace Spike
{
    struct MaterialCbuffer
    {
        glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
        int AlbedoTexToggle = 0;
        float Shininess = 32.0f;
        glm::vec3 __Padding;
    };

    class Material : public RefCounted
    {
    public:
        Material() = default;
        Material(const Ref<Shader>& shader);
        ~Material() = default;

        void Bind(Uint index);

        Ref<Shader>& GetShader() { return m_Shader; }

        Vector<Ref<Texture2D>>& GetTextures() { return m_Textures; }
        void PushTexture(const Ref<Texture2D>& tex, Uint slot = 0);

        glm::vec3& GetColor() { return m_Color; }
        void SetColor(const glm::vec3& color) { m_Color = color; }

        void SetDiffuseTexToggle(bool value);
        bool GetDiffuseTexToggle() { return m_AlbedoTexToggle; }
        void FlipTextures(bool flip);
        static Ref<Material> Material::Create(const Ref<Shader>& shader);
    public:
        float m_Shininess = 32.0f;
        glm::vec3 m_Color;
        bool m_AlbedoTexToggle;
        bool m_Flipped = false;

    private:
        Ref<Shader> m_Shader;
        Vector<Ref<Texture2D>> m_Textures;
        MaterialCbuffer m_CBufferData;
        Ref<ConstantBuffer> m_CBuffer;
    };
}