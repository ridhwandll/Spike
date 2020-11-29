#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

namespace LightEngine
{
    class SubTexture2D
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