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

out VertexOutput
{
    vec3 v_Normal;
    vec2 v_TexCoord;
    vec3 v_WorldPos;
} vsOut;

void main()
{
    vsOut.v_WorldPos = vec3(u_Transform * vec4(a_Position, 1.0));
    gl_Position = u_ViewProjection * vec4(vsOut.v_WorldPos, 1.0f);
    vsOut.v_TexCoord = a_TexCoord;
    vsOut.v_Normal = a_Normal;
}

#type fragment
#version 450 core

out vec4 FragColor;

struct AmbientLight
{
    vec3 Color;
    float Intensity;
};

struct DirectionalLight
{
    vec3 Direction;
    float __Padding0;

    vec3 Color;
    float Intensity;
};

struct PointLight
{
    vec3 Position;
    float __Padding0;

    vec3 Color;
    float __Padding1;

    float Intensity;
    float Constant;
    float Linear;
    float Quadratic;
};

in VertexOutput
{
    vec3 v_Normal;
    vec2 v_TexCoord;
    vec3 v_WorldPos;
} vsIn;

layout (std140, binding = 2) uniform Material
{
    uniform vec3  u_MatColor;
    uniform int   u_MatDiffuseTexToggle;

    uniform float u_MatShininess;
    uniform float u_MatSmoothness;
    uniform vec2 _Padding;
};

layout (std140, binding = 3) uniform Lights
{
    uniform vec3 u_CameraPosition;
    int __Padding;

    uniform int  u_AmbientLightCount;
    uniform int  u_DirectionalLightCount;
    uniform int  u_PointLightCount;
    int ___Padding;

    uniform PointLight u_PointLights[100];
    uniform AmbientLight u_AmbientLights[100];
    uniform DirectionalLight u_DirectionalLights[10];
};

uniform sampler2D u_DiffuseTexture; //I hate this

// Lightning Functions
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_MatShininess);

    vec3 diffuse = light.Color * diff * u_MatColor * light.Intensity;
    vec3 specular = light.Color * spec * u_MatColor * u_MatSmoothness;
    return diffuse + specular;

    return diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - vsIn.v_WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_MatShininess) * light.Intensity;

    float distance = length(light.Position - vsIn.v_WorldPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    vec3 diffuse = light.Color * diff * u_MatColor * light.Intensity;
    vec3 specular = light.Color * spec * u_MatColor * u_MatSmoothness;

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}
void main()
{
    vec3 norm = normalize(vsIn.v_Normal);
    vec3 viewDir = normalize(u_CameraPosition - vsIn.v_WorldPos);
    vec3 lightingResult = vec3(1);

    if (u_AmbientLightCount > 0 || u_DirectionalLightCount > 0 || u_PointLightCount > 0)
        lightingResult = vec3(0);

    for (int i = 0; i < u_AmbientLightCount; i++)
        lightingResult += u_AmbientLights[i].Color * vec3(u_AmbientLights[i].Intensity);

    for (int i = 0; i < u_DirectionalLightCount; i++)
        lightingResult += CalculateDirectionalLight(u_DirectionalLights[i], norm, viewDir);

    for (int i = 0; i < u_PointLightCount; i++)
        lightingResult += CalculatePointLight(u_PointLights[i], norm, viewDir);

    vec4 tex;
    if (u_MatDiffuseTexToggle == 1)
        tex = texture(u_DiffuseTexture, vsIn.v_TexCoord) * vec4(u_MatColor, 1.0f);
    else
        tex = vec4(u_MatColor, 1.0f);

    FragColor = tex * vec4(lightingResult, 1.0f);
}
