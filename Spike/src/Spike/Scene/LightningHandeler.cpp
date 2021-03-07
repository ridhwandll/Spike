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

namespace Spike
{
    LightningHandeler::LightningHandeler()
    {

    }

    LightningHandeler::~LightningHandeler()
    {

    }

    void LightningHandeler::Rock(const EditorCamera& editorCam, Ref<Material>& material)
    {
        Ref<Shader>& shader = material->GetShader();
        shader->Bind();
        shader->SetFloat3("u_CameraPosition", editorCam.GetPosition());
        shader->SetInt("u_AmbientLightCount", m_AmbientLights.size());
        shader->SetInt("u_PointLightCount", m_PointLights.size());

        for (int i = 0; i < m_AmbientLights.size(); i++)
        {
            auto& light = m_AmbientLights[i];
            shader->SetFloat ("u_AmbientLights[" + std::to_string(i) + "].Intensity", light.Intensity);
            shader->SetFloat3("u_AmbientLights[" + std::to_string(i) + "].Color",     light.Color);
        }

        for (int i = 0; i < m_DirectionalLights.size(); i++)
        {
            auto& light = m_DirectionalLights[i];
            shader->SetFloat("u_DirectionalLights[" + std::to_string(i) + "].Intensity", light.Intensity);
            shader->SetFloat3("u_DirectionalLights[" + std::to_string(i) + "].Color", light.Color);
            shader->SetFloat3("u_DirectionalLights[" + std::to_string(i) + "].Direction", light.Direction);
        }

        for (int i = 0; i < m_PointLights.size(); i++)
        {
            auto& light = m_PointLights[i];
            shader->SetFloat ("u_PointLights[" + std::to_string(i) + "].Intensity", light.Intensity);
            shader->SetFloat3("u_PointLights[" + std::to_string(i) + "].Position",  light.Position);
            shader->SetFloat3("u_PointLights[" + std::to_string(i) + "].Color",     light.Color);
            shader->SetFloat ("u_PointLights[" + std::to_string(i) + "].Constant",  light.Constant);
            shader->SetFloat ("u_PointLights[" + std::to_string(i) + "].Quadratic", light.Quadratic);
            shader->SetFloat ("u_PointLights[" + std::to_string(i) + "].Linear",    light.Linear);
        }
    }
}