/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Mesh
File Type      : cpp
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
#include "spkpch.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Spike/EngineAssets/EngineAssets.h"
#include "Spike/Core/Vault.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Spike
{
    glm::mat4 AssimpMat4ToGlmMat4(const aiMatrix4x4& matrix)
    {
        glm::mat4 result;
        result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
        result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
        result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
        result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
        return result;
    }

    static const uint32_t s_MeshImportFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices;

    Mesh::Mesh(const String& filepath)
        :m_FilePath(filepath)
    {
        SPK_CORE_LOG_INFO("Loading Mesh... %s", filepath.c_str());

        auto importer = CreateScope<Assimp::Importer>();
        const aiScene* scene = importer->ReadFile(filepath, s_MeshImportFlags);
        if (!scene || !scene->HasMeshes()) SPK_CORE_LOG_ERROR("Failed to load mesh file: %s", filepath.c_str());

        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;

        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11: m_Shader = Vault::GetBuiltInShaderFromCache("MeshShader.hlsl"); break;
            case RendererAPI::API::OpenGL: m_Shader = Vault::GetBuiltInShaderFromCache("MeshShader.glsl"); break;
        }
        m_Shader->Bind();

        m_Submeshes.reserve(scene->mNumMeshes);
        for (size_t m = 0; m < scene->mNumMeshes; m++)
        {
            aiMesh* mesh = scene->mMeshes[m];

            Submesh& submesh = m_Submeshes.emplace_back();
            submesh.BaseVertex = vertexCount;
            submesh.BaseIndex = indexCount;
            submesh.MaterialIndex = mesh->mMaterialIndex;
            submesh.IndexCount = mesh->mNumFaces * 3;
            submesh.VertexCount = mesh->mNumVertices;
            submesh.MeshName = mesh->mName.C_Str();
            submesh.CBuffer = ConstantBuffer::Create(m_Shader, "Mesh", nullptr, sizeof(glm::mat4), 1, ShaderDomain::VERTEX, DataUsage::DYNAMIC);
            vertexCount += submesh.VertexCount;
            indexCount += submesh.IndexCount;

            SPK_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
            SPK_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

            for (size_t i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
                vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

                if (mesh->HasTextureCoords(0))
                    vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                else
                    vertex.TexCoord = { 0.0f, 0.0f };
                m_Vertices.push_back(vertex);
            }

            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                SPK_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Mesh Must have 3 indices!");
                Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
                m_Indices.push_back(index);
            }
        }

        TraverseNodes(scene->mRootNode);

        VertexBufferLayout layout =
        {
            { ShaderDataType::Float3, "M_POSITION" },
            { ShaderDataType::Float3, "M_NORMAL" },
            { ShaderDataType::Float2, "M_TEXCOORD" },
        };

        m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex), layout);
        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), std::size(m_Indices) * 3);

        PipelineSpecification spec;
        spec.Shader = m_Shader;
        spec.VertexBuffer = m_VertexBuffer;
        spec.IndexBuffer = m_IndexBuffer;
        m_Pipeline = Pipeline::Create(spec);
    }

    void Mesh::TraverseNodes(aiNode* node, const glm::mat4& parentTransform, uint32_t level)
    {
        glm::mat4 localTransform = AssimpMat4ToGlmMat4(node->mTransformation);
        glm::mat4 transform = parentTransform * localTransform;
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            uint32_t mesh = node->mMeshes[i];
            auto& submesh = m_Submeshes[mesh];
            submesh.NodeName = node->mName.C_Str();
            submesh.Transform = transform;
            submesh.LocalTransform = localTransform;
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
            TraverseNodes(node->mChildren[i], transform, level + 1);
    }
}
