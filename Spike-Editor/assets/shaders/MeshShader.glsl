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
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_ObjectID; //BIG TODO: Make it work! (mousepicking) this doesn't give error. The shader is fine!

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec2 v_TexCoord;
out flat int v_ObjectID;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
    v_ObjectID = a_ObjectID;
}

#type fragment
#version 430 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int o_IDBuffer;

in vec3 v_Normal;
in vec2 TexCoords;
in flat int v_ObjectID;

uniform sampler2D textureDiffuse;

void main()
{
    FragColor = vec4((v_Normal * 0.5 + 0.5), 1.0);/*vec4(1.0f, 0.5f, 0.0f, 1.0f);*///texture(textureDiffuse, TexCoords);
    o_IDBuffer = v_ObjectID;
}