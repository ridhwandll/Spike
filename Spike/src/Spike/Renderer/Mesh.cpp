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
#include "Spike/Renderer/Mesh.h"
#include <stb_image.h>
#include <glad/glad.h>
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
            glActiveTexture(GL_TEXTURE0 + i);
            String number;
            String name = m_Textures[i].Type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader->SetInt((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].ID);
        }
        glActiveTexture(GL_TEXTURE0);

        m_VertexArray->Bind();
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    }

    void Submesh::SetupMesh()
    {
        m_VertexArray = VertexArray::Create();
        m_VertexBuffer = VertexBuffer::Create(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(&m_Indices[0], m_Indices.size() * sizeof(uint32_t));

        VertexBufferLayout layout =
        {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoords" },
        };

        m_VertexBuffer->SetLayout(layout);

        m_VertexArray->Bind();
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        m_VertexArray->Unbind();
    }

    uint32_t TextureFromFile(const char* path, const String& directory, bool gamma = false);
    Mesh::Mesh(const String& path)
    {
        m_Shader = Shader::Create("Spike-Editor/assets/shaders/MeshShader.glsl");
        LoadMesh(path);
    }

    void Mesh::Draw()
    {
        for (uint32_t i = 0; i < m_Meshes.size(); i++)
            m_Meshes[i].Draw(m_Shader);
    }

    void Mesh::LoadMesh(String path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            SPK_CORE_LOG_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
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
            m_Meshes.push_back(ProcessMesh(mesh, scene));
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
        Vector<TextureStruct> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        Vector<TextureStruct> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        return Submesh(vertices, indices, textures);
    }

    Vector<TextureStruct> Mesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const String& typeName)
    {
        Vector<TextureStruct> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
            {
                if (std::strcmp(m_TexturesLoaded[j].Path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(m_TexturesLoaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                TextureStruct texture;
                texture.ID = TextureFromFile(str.C_Str(), m_FilePath);
                texture.Type = typeName;
                texture.Path = str.C_Str();
                textures.push_back(texture);
                m_TexturesLoaded.push_back(texture); // add to loaded textures
            }
        }
        return textures;
    }

    uint32_t TextureFromFile(const char* name, const String& directory, bool gamma)
    {
        std::filesystem::path path = directory;
        auto parentPath = path.parent_path();
        parentPath /= std::string(name);
        std::string texturePath = parentPath.string();

        unsigned int textureID;
        glGenTextures(1, &textureID);

        //stbi_set_flip_vertically_on_load(1);
        int width, height, nrComponents;
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << name << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
}
