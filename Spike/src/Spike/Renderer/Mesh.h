/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Mesh
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/Shader.h"
#include "Spike/Renderer/Pipeline.h"
#include "Spike/Renderer/VertexBuffer.h"
#include "Spike/Renderer/IndexBuffer.h"
#include "Spike/Renderer/ConstantBuffer.h"
#include "Spike/Renderer/Material.h"
#include <glm/glm.hpp>

struct aiMesh;
struct aiNode;
struct aiMaterial;
enum aiTextureType;

namespace Spike
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };

    struct Submesh
    {
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t MaterialIndex;
        uint32_t IndexCount;
        uint32_t VertexCount;

        Ref<ConstantBuffer> CBuffer;

        glm::mat4 Transform;
        glm::mat4 LocalTransform;
        String NodeName, MeshName;
    };

    struct Index
    {
        uint32_t V1, V2, V3;
    };

    class Mesh : public RefCounted
    {
    public:
        Mesh(const String& filename);

        Ref<Pipeline> GetPipeline() { return m_Pipeline; }
        Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
        Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

        Vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
        const Vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }

        Ref<Material>& GetMaterial() { return m_Material; }
        const Vector<Vertex>& GetVertices() const { return m_Vertices; }
        const Vector<Index>& GetIndices() const { return m_Indices; }

        Ref<Shader> GetShader() { return m_Shader; }
        const String& GetFilePath() const { return m_FilePath; }
    private:
        void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);

    private:
        Vector<Submesh> m_Submeshes;

        Ref<Pipeline> m_Pipeline;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;

        Vector<Vertex> m_Vertices;
        Vector<Index> m_Indices;

        Ref<Shader> m_Shader;
        Ref<Material> m_Material;

        String m_FilePath;
        friend class Renderer;
    };
}