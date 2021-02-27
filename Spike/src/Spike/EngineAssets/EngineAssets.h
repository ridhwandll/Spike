/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : GLSLShaders
File Type      : h
File created on: 2021/02/06
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Core/Base.h"

SPK_NAMESPACE_BEGIN
static String s_GLSLMeshShader =
R"(
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform int u_EntityID;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_WorldPos;
out int v_EntityID;

void main()
{
    v_WorldPos = vec3(u_Transform * vec4(a_Position, 1.0));
    gl_Position = u_ViewProjection * vec4(v_WorldPos, 1.0f);
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
    v_EntityID = u_EntityID;
}

#type fragment
#version 450 core

out vec4 FragColor;
out int entityID;

in vec3 v_Normal;
in vec3 v_WorldPos;
in vec2 v_TexCoord;
in flat int v_EntityID;

/*Uniforms*/
uniform sampler2D TextureAlbedo1;

/*Main Function*/
void main()
{
    FragColor = texture(TextureAlbedo1, v_TexCoord);
    entityID = v_EntityID;
}

)";

static String s_GLSLRenderer2DShader =
R"(
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout (std140, binding = 0) uniform Data
{
    uniform mat4 u_ViewProjection;
};

out vec4 v_Color;
out vec2 v_TexCoord;
out flat float v_TexIndex;
out float v_TilingFactor;
out int v_EntityID;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

out vec4 color;
out int entityID;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor = v_Color;
    switch(int(v_TexIndex))
    {
        case  0: texColor *= texture(u_Textures[ 0], v_TexCoord * v_TilingFactor); break;
        case  1: texColor *= texture(u_Textures[ 1], v_TexCoord * v_TilingFactor); break;
        case  2: texColor *= texture(u_Textures[ 2], v_TexCoord * v_TilingFactor); break;
        case  3: texColor *= texture(u_Textures[ 3], v_TexCoord * v_TilingFactor); break;
        case  4: texColor *= texture(u_Textures[ 4], v_TexCoord * v_TilingFactor); break;
        case  5: texColor *= texture(u_Textures[ 5], v_TexCoord * v_TilingFactor); break;
        case  6: texColor *= texture(u_Textures[ 6], v_TexCoord * v_TilingFactor); break;
        case  7: texColor *= texture(u_Textures[ 7], v_TexCoord * v_TilingFactor); break;
        case  8: texColor *= texture(u_Textures[ 8], v_TexCoord * v_TilingFactor); break;
        case  9: texColor *= texture(u_Textures[ 9], v_TexCoord * v_TilingFactor); break;
        case 10: texColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
        case 11: texColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
        case 12: texColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
        case 13: texColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
        case 14: texColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
        case 15: texColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
        case 16: texColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
        case 17: texColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
        case 18: texColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
        case 19: texColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
        case 20: texColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
        case 21: texColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
        case 22: texColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
        case 23: texColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
        case 24: texColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
        case 25: texColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
        case 26: texColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
        case 27: texColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
        case 28: texColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
        case 29: texColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
        case 30: texColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
        case 31: texColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
    }
    color = texColor;
    entityID = v_EntityID;
}
)";

static String s_HLSLRenderer2DShader =
R"(
#type vertex
#pragma pack_matrix(row_major)

cbuffer Data : register(b0)
{
    matrix uViewProjection; //Camera
}

struct vsIn
{
    float4 vPosition     : POSITION;
    float4 vColor        : COLOR;
    float2 vTexCoord     : TEXCOORD;
    float  vTexIndex     : TEXINDEX;
    float  vTilingFactor : TILINGFACTOR;
};

struct vsOut
{
    float4 vPosition     : SV_POSITION;
    float4 vColor        : COLOR;
    float2 vTexCoord     : TEXCOORD;
    float  vTexIndex     : TEXINDEX;
    float  vTilingFactor : TILINGFACTOR;
};

vsOut main(vsIn input)
{
    vsOut output;
    output.vPosition = mul(input.vPosition, uViewProjection);

    output.vColor = input.vColor;
    output.vTexCoord = input.vTexCoord;
    output.vTexIndex = input.vTexIndex;
    output.vTilingFactor = input.vTilingFactor;
    return output;
}

#type pixel
struct vsOut
{
    float4 vPosition     : SV_POSITION;
    float4 vColor        : COLOR;
    float2 vTexCoord     : TEXCOORD;
    float  vTexIndex     : TEXINDEX;
    float  vTilingFactor : TILINGFACTOR;
};

Texture2D textures[32];
SamplerState sampleType : register(s0);

float4 main(vsOut input) : SV_TARGET
{

    switch ((int)input.vTexIndex)
    {
        case  0: return textures[ 0].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  1: return textures[ 1].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  2: return textures[ 2].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  3: return textures[ 3].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  4: return textures[ 4].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  5: return textures[ 5].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  6: return textures[ 6].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  7: return textures[ 7].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  8: return textures[ 8].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case  9: return textures[ 9].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 10: return textures[10].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 11: return textures[11].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 12: return textures[12].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 13: return textures[13].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 14: return textures[14].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 15: return textures[15].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 16: return textures[16].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 17: return textures[17].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 18: return textures[18].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 19: return textures[19].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 20: return textures[20].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 21: return textures[21].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 22: return textures[22].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 23: return textures[23].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 24: return textures[24].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 25: return textures[25].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 26: return textures[26].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 27: return textures[27].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 28: return textures[28].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 29: return textures[29].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 30: return textures[30].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        case 31: return textures[31].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
        default: return textures[ 0].Sample(sampleType, input.vTexCoord * input.vTilingFactor) * input.vColor; break;
    }
}
)";

static String s_HLSLMeshShader =
R"(
#type vertex
#pragma pack_matrix(row_major)

cbuffer Camera : register(b0)
{
    matrix u_ViewProjection;
}

cbuffer Mesh : register(b1)
{
    matrix u_Transform;
}

struct vsIn
{
    float3 v_Position : M_POSITION;
    float3 v_Normal   : M_NORMAL;
    float2 v_TexCoord : M_TEXCOORD;
};

struct vsOut
{
    float4 v_Position : SV_POSITION;
    float3 v_Normal   : M_NORMAL;
    float2 v_TexCoord : M_TEXCOORD;
};

vsOut main(vsIn input)
{
    vsOut output;
    output.v_Position = mul(float4(input.v_Position, 1.0f), u_Transform);
    output.v_Position = mul(output.v_Position, u_ViewProjection);

    output.v_Normal = input.v_Normal;
    output.v_TexCoord = input.v_TexCoord;
    return output;
}

#type pixel

struct vsOut
{
    float4 v_Position : SV_POSITION;
    float3 v_Normal : M_NORMAL;
    float2 v_TexCoord : M_TEXCOORD;
};

float4 main(vsOut input) : SV_TARGET
{
    float4 color = float4((input.v_Normal * 0.5 + 0.5), 1.0);
    return color;
}
)";
SPK_NAMESPACE_END