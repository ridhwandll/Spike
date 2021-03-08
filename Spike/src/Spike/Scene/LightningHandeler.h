/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : LightningHandeler
File Type      : h
File created on: 2021/03/07
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/Material.h"
#include "Spike/Renderer/EditorCamera.h"
#include "Spike/Renderer/ConstantBuffer.h"
#include "Spike/Scene/Components.h"
#include <glm/glm.hpp>

namespace Spike
{
    struct AmbientLight
    {
        glm::vec3 Color;
        float Intensity;
    };

    struct DirectionalLight
    {
        glm::vec3 Direction;
        int __Padding0;

        glm::vec3 Color;
        float Intensity;
    };

    struct PointLight
    {
        glm::vec3 Position;
        int __Padding0;

        glm::vec3 Color;
        float __Padding1;

        float Intensity;
        float Constant;
        float Linear;
        float Quadratic;
    };

    struct LightCBuffer
    {
        glm::vec3 CameraPosition;
        float __Padding0;

        int AmbientLightCount;
        int DirectionalLightCount;
        int PointLightCount;
        int __Padding1;

        PointLight PointLights[100];
        AmbientLight AmbientLights[100];
        DirectionalLight DirectionalLights[10];
    };

    class LightningHandeler
    {
    public:
        LightningHandeler();
        ~LightningHandeler();

        Vector<AmbientLight> m_AmbientLights;
        Vector<PointLight> m_PointLights;
        Vector<DirectionalLight> m_DirectionalLights;

    public:
        void CalculateAndRenderLights(const glm::vec3& cameraPos, Ref<Material>& material);
        void ClearLights();

    private:
        Ref<ConstantBuffer> m_LightConstantBuffer;
        LightCBuffer m_LightCBufferData;
    };
}