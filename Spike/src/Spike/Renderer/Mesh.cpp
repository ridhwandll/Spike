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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Spike/Core/Vault.h"
#include "Spike/Renderer/Mesh.h"
#include "Renderer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

namespace Spike
{
    Submesh::Submesh(Vector<Vertex> vertices, Vector<uint32_t> indices, Vector<TextureStruct> textures)
        :m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
    {
        SetupMesh();
    }

    void Submesh::Draw(Ref<Shader>& shader)
    {
        uint32_t diffuseNr = 1;
        uint32_t specularNr = 1;
        for (uint32_t i = 0; i < m_Textures.size(); i++)
        {
            m_Textures[i].Texture->ActivateSlot(i);
            String number;
            String name = m_Textures[i].Type;
            if (name == "TextureAlbedo")
                number = std::to_string(diffuseNr++);
            else if (name == "TextureSpecular")
                number = std::to_string(specularNr++);

            shader->SetInt(name + number, i);
            m_Textures[i].Texture->Bind(i);
        }

        m_VertexArray->Bind();
        Renderer::Submit(m_VertexArray, m_Indices.size());
    }

    void Submesh::SetupMesh()
    {
        m_VertexArray = VertexArray::Create();
        m_VertexBuffer = VertexBuffer::Create(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(&m_Indices[0], m_Indices.size() * sizeof(uint32_t));

        VertexBufferLayout layout =
        {
            { ShaderDataType::Float3, "a_Position"  },
            { ShaderDataType::Float3, "a_Normal"    },
            { ShaderDataType::Float2, "a_TexCoords" },
        };

        m_VertexBuffer->SetLayout(layout);

        m_VertexArray->Bind();
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        m_VertexArray->Unbind();
    }
    Mesh::Mesh(const String& path)
    {
        m_Shader = Vault::GetBuiltInShaderFromCache("MeshShader");
        LoadMesh(path);
    }

    void Mesh::Draw(int entityID)
    {
        for (uint32_t i = 0; i < m_Submeshes.size(); i++)
            m_Submeshes[i].Draw(m_Shader);
    }

    void Mesh::Reload()
    {
        Clear();
        LoadMesh(m_FilePath);
    }

    void Mesh::Clear()
    {
        m_Submeshes.clear();
        m_TexturesCache.clear();
    }

    void Mesh::LoadMesh(String path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            SPK_CORE_LOG_ERROR("Assimp Error: %s", importer.GetErrorString());
            return;
        }
        m_FilePath = path.substr(0, path.find_last_of('/'));
        ProcessNode(scene->mRootNode, scene);
    }

    void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Submeshes.push_back(ProcessMesh(mesh, scene));
        }
        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Submesh Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        Vector<Vertex> vertices;
        Vector<uint32_t> indices;
        Vector<TextureStruct> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            // texture coordinates
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse maps
        Vector<TextureStruct> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "TextureAlbedo");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        Vector<TextureStruct> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "TextureSpecular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        return Submesh(vertices, indices, textures);
    }

    Vector<TextureStruct> Mesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const String& typeName)
    {
        Vector<TextureStruct> textures;
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (uint32_t j = 0; j < m_TexturesCache.size(); j++)
            {
                if (std::strcmp(m_TexturesCache[j].Path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(m_TexturesCache[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                TextureStruct texture;
                texture.Texture = TextureFromFile(str.C_Str(), m_FilePath);
                texture.Type = typeName;
                texture.Path = str.C_Str();
                textures.push_back(texture);
                m_TexturesCache.push_back(texture);
            }
        }
        return textures;
    }

    Ref<Texture2D> Mesh::TextureFromFile(const char* name, const String& directory)
    {
        std::filesystem::path path = directory;
        auto parentPath = path.parent_path();
        parentPath /= std::string(name);
        std::string texturePath = parentPath.string();

        Ref<Texture2D> texture = Texture2D::Create(texturePath.c_str(), m_FlipTexturesVertically, m_SRGB);
        Vault::SubmitTexture2D(texture);
        return texture;
    }
}
