/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Renderer2D
File Type      : cpp
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
#include "spkpch.h"
#include "Renderer2D.h"
#include "Spike/Core/Vault.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Spike/EngineAssets/EngineAssets.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Spike
{
    struct ShaderConstantBuffer
    {
        glm::mat4 ViewProjection;
    };

    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        Ref<Pipeline> QuadPipeline;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
        Ref<ConstantBuffer> CBuffer;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        glm::vec4 QuadVertexPositions[4];
        Renderer2D::Statistics Stats;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        s_Data.TextureShader = Shader::AddBuiltInShader(s_HLSLRenderer2DShader.c_str(), "Standard2D.hlsl");
        Vault::SubmitBuiltInShader(s_Data.TextureShader); //Submit the shader to vault

        s_Data.TextureShader->Bind();
        s_Data.CBuffer = ConstantBuffer::Create(s_Data.TextureShader, "Data", nullptr, sizeof(ShaderConstantBuffer), 0, ShaderDomain::VERTEX, DataUsage::DYNAMIC);

        /* [Spike] Vertex Buffer [Spike] */
        VertexBufferLayout layout =
        {
            { ShaderDataType::Float3, "POSITION"     },
            { ShaderDataType::Float4, "COLOR"        },
            { ShaderDataType::Float2, "TEXCOORD"     },
            { ShaderDataType::Float,  "TEXINDEX"     },
            { ShaderDataType::Float,  "TILINGFACTOR" },
        };
        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex), layout);

        /* [Spike] Index Buffer [Spike] */
        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        quadIB->Bind();

        /* [Spike] Textures [Spike] */
        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        //sData.TextureShader->SetIntArray("u_Textures", samplers, sData.MaxTextureSlots);
        // Set first texture slot to 0
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        PipelineSpecification spec = {};
        spec.Shader = s_Data.TextureShader;
        spec.IndexBuffer = quadIB;
        spec.VertexBuffer = s_Data.QuadVertexBuffer;
        s_Data.QuadPipeline = Pipeline::Create(spec);
        s_Data.QuadPipeline->SetPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
        s_Data.QuadPipeline->Bind();
        delete[] quadIndices;
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        s_Data.TextureShader->Bind();
        s_Data.CBuffer->SetData(&viewProj);
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        StartBatch();
    }

    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        glm::mat4 viewProj = camera.GetViewProjection();
        s_Data.TextureShader->Bind();
        s_Data.CBuffer->SetData(&viewProj);
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        StartBatch();
    }

    void Renderer2D::EndScene() { Flush(); }

    void Renderer2D::StartBatch()
    {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::Flush()
    {
        if (s_Data.QuadIndexCount == 0)
            return; // Nothing to draw

        uint32_t dataSize = (uint32_t)((byte*)s_Data.QuadVertexBufferPtr - (byte*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        // Bind textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i, ShaderDomain::PIXEL);

        RenderCommand::DrawIndexed(s_Data.QuadPipeline, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float tilingFactor = 1.0f;

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            Flush();
            StartBatch();
        }

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f } };

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            Flush();
            StartBatch();
        }

        float textureSlot = 0.0f;
        /* [Spike] For each texture in the sData.TextureSlotIndex if *sData.TextureSlots[i]
         *  is equal to the given texture grab that texture slot and set it [Spike] */
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i] == *texture)
            {
                textureSlot = (float)i;
                break;
            }
        }

        if (textureSlot == 0.0f)
        {
            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            {
                Flush();
                StartBatch();
            }

            textureSlot = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureSlot;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite)
    {
        if (sprite.Texture)
            DrawQuad(transform, sprite.Texture, sprite.TilingFactor, sprite.Color);
        else
            DrawQuad(transform, sprite.Color);
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
}