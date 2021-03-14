//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Pipeline.h"
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/ConstantBuffer.h"
#include <glm/glm.hpp>

namespace Spike
{
    class Skybox : public RefCounted
    {
    public:
        Skybox() = default;
        Skybox(const Ref<TextureCube>& texture);
        ~Skybox() = default;

        void Render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
        void SetCubemapTexture(const Ref<TextureCube>& texture) { m_Texture = texture; }

        /*
        Get some cool skyboxes from http://www.humus.name/index.php?page=Textures
        Be sure to rename the image files as Spike Engine needs them to be,
        For more information on the Spike-Skybox naming see the TextureCube class in Texture.h file
        */
        static Ref<Skybox> Create(const Ref<TextureCube>& texture);
    private:
        Ref<ConstantBuffer> m_SkyboxCBuffer;
        Ref<Pipeline> m_Pipeline;
        Ref<TextureCube> m_Texture;
    };
}