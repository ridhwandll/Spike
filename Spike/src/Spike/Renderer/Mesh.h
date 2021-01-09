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

namespace Spike
{
    class Mesh : public RefCounted
    {
    public:
        struct Vertex
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            //glm::vec2 Texcoord;
        };
        static_assert(sizeof(Vertex) == 6 * sizeof(float));
        static const int NumAttributes = 5;

        struct Index
        {
            uint32_t V1, V2, V3;
        };

        static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

        Mesh(const std::string& filename);
        ~Mesh();

        void OnUpdate(Timestep ts);
        void Draw(const glm::mat4& transform);

        const std::string& GetFilePath() const { return m_FilePath; }
        Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }
        Ref<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
        Ref<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

    private:
        void DumpVertexBuffer();
    private:
        std::vector<Vertex> m_Vertices;
        std::vector<Index> m_Indices;

        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Shader> m_MeshShader;

        std::string m_FilePath;
    };


}