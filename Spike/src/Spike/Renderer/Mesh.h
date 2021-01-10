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

namespace Assimp {
    class Importer;
}

namespace Spike
{
    class Mesh : public RefCounted
    {
    public:
        Mesh(const std::string& filepath);
        ~Mesh();

        struct Vertex
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 Texcoord;
        };
        static_assert(sizeof(Vertex) == 8 * sizeof(float));

        const std::string& GetFilePath() const { return m_FilePath; }
    public:
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Shader> m_MeshShader;

    private:
        void DumpVertexBuffer();
    private:
        Scope<Assimp::Importer> m_Importer;

        std::vector<Vertex> m_Vertices = {};
        std::vector<uint32_t> m_Indices = {};

        std::string m_FilePath;
    };
}