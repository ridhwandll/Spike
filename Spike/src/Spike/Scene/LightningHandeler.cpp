/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : LightningHandeler
File Type      : cpp
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
#include "LightningHandeler.h"
#include "Spike/Core/Vault.h"
#include "Spike/Renderer/RendererAPI.h"

namespace Spike
{
    LightningHandeler::LightningHandeler()
    {
        Ref<Shader> shader;
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11: shader = Vault::Get<Shader>("MeshShader.hlsl"); break;
            case RendererAPI::API::OpenGL: shader = Vault::Get<Shader>("MeshShader.glsl"); break;
        }

        //Setup the lights constant buffer
        m_LightConstantBuffer = ConstantBuffer::Create(shader, "Lights", nullptr, sizeof(LightCBuffer), 3, ShaderDomain::PIXEL, DataUsage::DYNAMIC);
    }

    LightningHandeler::~LightningHandeler()
    {

    }

    void LightningHandeler::CalculateAndRenderLights(const glm::vec3& cameraPos, Ref<Material>& material)
    {
        Ref<Shader>& shader = material->GetShader();
        shader->Bind();

        m_LightCBufferData.CameraPosition = cameraPos;
        m_LightCBufferData.SkyLightCount = m_SkyLights.size();
        m_LightCBufferData.PointLightCount = m_PointLights.size();

        for (int i = 0; i < m_PointLights.size(); i++)
        {
            auto& light = m_PointLights[i];
            m_LightCBufferData.PointLights[i].Position  = light.Position;
            m_LightCBufferData.PointLights[i].Color     = light.Color;
            m_LightCBufferData.PointLights[i].Intensity = light.Intensity;
            m_LightCBufferData.PointLights[i].Constant  = light.Constant;
            m_LightCBufferData.PointLights[i].Quadratic = light.Quadratic;
            m_LightCBufferData.PointLights[i].Linear    = light.Linear;
        }

        for (int i = 0; i < m_SkyLights.size(); i++)
        {
            auto& light = m_SkyLights[i];
            m_LightCBufferData.AmbientLights[i].Intensity = light.Intensity;
            m_LightCBufferData.AmbientLights[i].Color = light.Color;
        }

        m_LightConstantBuffer->SetData(&m_LightCBufferData);
    }

    void LightningHandeler::ClearLights()
    {
        m_SkyLights.clear();
        m_PointLights.clear();
    }
}