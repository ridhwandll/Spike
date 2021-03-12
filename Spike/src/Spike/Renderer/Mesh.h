//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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
        Uint BaseVertex;
        Uint BaseIndex;
        Uint MaterialIndex;
        Uint IndexCount;
        Uint VertexCount;

        Ref<ConstantBuffer> CBuffer;

        glm::mat4 Transform;
        glm::mat4 LocalTransform;
        String NodeName, MeshName;
    };

    struct Index { Uint V1, V2, V3; };

    class Mesh : public RefCounted
    {
    public:
        Mesh(const String& filename);
        Mesh(const Vector<Vertex>& vertices, const Vector<Index>& indices, const glm::mat4& transform);

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
        void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), Uint level = 0);

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
    };
}