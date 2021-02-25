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
#include <glm/glm.hpp>

struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiScene;
enum aiTextureType;

namespace Spike
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct TextureStruct
    {
        Ref<Spike::Texture> Texture;
        String Type;
        String Path;
    };

    class Submesh
    {
    public:
        Submesh(Vector<Vertex> vertices, Vector<uint32_t> indices, Vector<TextureStruct> textures);
        void Draw(Ref<Shader>& shader);

    public:
        Vector<Vertex>        m_Vertices;
        Vector<uint32_t>      m_Indices;
        Vector<TextureStruct> m_Textures;

    private:
        Ref<Pipeline> m_Pipeline;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        void SetupMesh();
    };

    class Mesh : public RefCounted
    {
    public:
        Mesh(const String& path);
        void Draw(int entityID = -1);
        void Reload();
        Ref<Shader>& GetShader() { return m_Shader; }
        void Clear();

    public:
        String m_FilePath;
        bool m_FlipTexturesVertically = true;
        bool m_SRGB = true;
    private:
        Vector<Submesh> m_Submeshes;
        Vector<TextureStruct> m_TexturesCache;
        Ref<Shader> m_Shader;
    private:
        Ref<Texture2D> TextureFromFile(const char* name, const String& directory);
        void LoadMesh(String path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Submesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        Vector<TextureStruct> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const String& typeName);
        friend class SceneHierarchyPanel;
    };
}