/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--
-------------------------SPIKE ENGINE MESH SHADER------------------------------
         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : MeshShader
File Type      : glsl
File created on: 2020/11/01
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout (std140, binding = 0) uniform Camera
{
    uniform mat4 u_ViewProjection;
};

layout (std140, binding = 1) uniform Mesh
{
    uniform mat4 u_Transform;
};

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_WorldPos;

void main()
{
    v_WorldPos = vec3(u_Transform * vec4(a_Position, 1.0));
    gl_Position = u_ViewProjection * vec4(v_WorldPos, 1.0f);
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
}

#type fragment
#version 450 core

out vec4 FragColor;

in vec3 v_Normal;
in vec3 v_WorldPos;
in vec2 v_TexCoord;

layout (std140, binding = 2) uniform Material
{
    uniform vec3 u_Color;
    uniform int u_AlbedoTexToggle;
};

uniform sampler2D u_Texture;

void main()
{
    if (u_AlbedoTexToggle == 1)
        FragColor = texture(u_Texture, v_TexCoord);
    else
        FragColor = vec4(u_Color, 1.0f);
}

