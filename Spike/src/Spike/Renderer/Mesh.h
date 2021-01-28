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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Core/Timestep.h"
#include "Spike/Renderer/Renderer.h"
#include "Spike/Renderer/Shader.h"
#include "Spike/Renderer/VertexArray.h"
#include "Spike/Renderer/VertexBuffer.h"
#include "Spike/Renderer/IndexBuffer.h"
#include <glm/glm.hpp>

struct aiNode;
namespace Assimp {
    class Importer;
}

namespace Spike
{
    class Submesh
    {
    public:
        uint32_t BaseVertex = 0;
        uint32_t BaseIndex = 0;
        //uint32_t MaterialIndex = 0;
        uint32_t IndexCount = 0;

        glm::mat4 Transform = glm::mat4(1.0f);
        String NodeName = "", MeshName = "";
    };

    class Mesh : public RefCounted
    {
    public:
        struct Vertex
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 Texcoord;
            int ObjectID;
        };
        static_assert(sizeof(Vertex) == 9 * sizeof(float));

        struct Index
        {
            uint32_t V1, V2, V3;
        };
        static_assert(sizeof(Index) == 3 * sizeof(uint32_t));


        Mesh(const String& filepath);
        Mesh(const String& filepath, uint32_t entityID); //BIG TODO: Sort this out. Make mousepicking work with 3D!
        Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
        ~Mesh();

        int m_ObjectID;
        std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
        const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }
    public:
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Shader> m_MeshShader;
        std::vector<Submesh> m_Submeshes;
        String m_FilePath;
    private:
        void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);
        void Generate(const String& filepath, uint32_t entityID = 0);
        void DumpVertexBuffer();

    private:
        Scope<Assimp::Importer> m_Importer;

        std::vector<Vertex> m_Vertices = {};
        std::vector<Index> m_Indices = {};
    };
}