//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/Camera.h"
#include "Spike/Renderer/EditorCamera.h"
#include "Spike/Scene/Components.h"

namespace Spike::Renderer2D
{
    void Init();
    void Shutdown();

    void BeginScene(const Camera& camera, const glm::mat4& transform);
    void BeginScene(const EditorCamera& camera);
    void EndScene();

    void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
    void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite);
    void DrawDebugQuad(const glm::mat4& transform);
    void Flush();

    struct Statistics
    {
        Uint DrawCalls = 0;
        Uint QuadCount = 0;
        Uint GetTotalVertexCount() const { return QuadCount * 4; }
        Uint GetTotalIndexCount() const { return QuadCount * 6; }
    };

    void UpdateStats();
    Statistics GetStats();
    void StartBatch();
}
