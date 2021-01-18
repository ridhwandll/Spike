/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

file Name      : MeshGenerator
file Type      : cpp
file created on: 2021/01/18
file created by: fahim fuad
Other editors  : None
Github repository : https://github.com/fahimfuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERfECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED fROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "MeshGenerator.h"

#define _USE_MATH_DEFINES
#include <math.h>

#pragma warning(push)
#pragma warning (disable : 4244) //Disable ugly 'C4244' "type conversion" warning!

namespace Spike
{
    Ref<Mesh> MeshGenerator::CreateCube(const glm::vec3& size)
    {
        std::vector<Mesh::Vertex> vertices;
        vertices.resize(8);
        vertices[0].Position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
        vertices[1].Position = { size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
        vertices[2].Position = { size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
        vertices[3].Position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
        vertices[4].Position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
        vertices[5].Position = { size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
        vertices[6].Position = { size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
        vertices[7].Position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };

        vertices[0].Normal = { -1.0f, -1.0f,  1.0f };
        vertices[1].Normal = { 1.0f, -1.0f,  1.0f };
        vertices[2].Normal = { 1.0f,  1.0f,  1.0f };
        vertices[3].Normal = { -1.0f,  1.0f,  1.0f };
        vertices[4].Normal = { -1.0f, -1.0f, -1.0f };
        vertices[5].Normal = { 1.0f, -1.0f, -1.0f };
        vertices[6].Normal = { 1.0f,  1.0f, -1.0f };
        vertices[7].Normal = { -1.0f,  1.0f, -1.0f };

        std::vector<Mesh::Index> indices;
        indices.resize(12);
        indices[0] = { 0, 1, 2 };
        indices[1] = { 2, 3, 0 };
        indices[2] = { 1, 5, 6 };
        indices[3] = { 6, 2, 1 };
        indices[4] = { 7, 6, 5 };
        indices[5] = { 5, 4, 7 };
        indices[6] = { 4, 0, 3 };
        indices[7] = { 3, 7, 4 };
        indices[8] = { 4, 5, 1 };
        indices[9] = { 1, 0, 4 };
        indices[10] = { 3, 2, 6 };
        indices[11] = { 6, 7, 3 };

        return Ref<Mesh>::Create(vertices, indices, glm::mat4(1.0F));
    }

    Ref<Mesh> MeshGenerator::CreateSphere(float radius)
    {
        std::vector<Mesh::Vertex> vertices;
        std::vector<Mesh::Index> indices;

        constexpr float latitudeBands = 30;
        constexpr float longitudeBands = 30;

        for (float latitude = 0.0F; latitude <= latitudeBands; latitude++)
        {
            float theta = latitude * M_PI / latitudeBands;
            float sinTheta = glm::sin(theta);
            float cosTheta = glm::cos(theta);

            for (float longitude = 0.0F; longitude <= longitudeBands; longitude++)
            {
                float phi = longitude * 2 * M_PI / longitudeBands;
                float sinPhi = glm::sin(phi);
                float cosPhi = glm::cos(phi);

                Mesh::Vertex vertex;
                vertex.Normal = { cosPhi * sinTheta, cosTheta, sinPhi * sinTheta };
                vertex.Position = { radius * vertex.Normal.x, radius * vertex.Normal.y, radius * vertex.Normal.z };
                vertices.push_back(vertex);
            }
        }

        for (uint32_t latitude = 0; latitude < latitudeBands; latitude++)
        {
            for (uint32_t longitude = 0; longitude < longitudeBands; longitude++)
            {
                uint32_t first = (latitude * (longitudeBands + 1)) + longitude;
                uint32_t second = first + longitudeBands + 1;
        
                indices.push_back( {first, second, first + 1} );
                indices.push_back( {second, second + 1, first + 1} );
            }
        }

        return Ref<Mesh>::Create(vertices, indices, glm::mat4(1.0F));
    }
}
#pragma warning (pop)