//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Material.h"
#include "Spike/Renderer/EditorCamera.h"
#include "Spike/Renderer/ConstantBuffer.h"
#include "Spike/Scene/Components.h"
#include <glm/glm.hpp>

namespace Spike
{
    struct SkyLight
    {
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

        int SkyLightCount;
        int PointLightCount;
        int __Padding1;
        int __Padding2;

        PointLight PointLights[100];
        SkyLight AmbientLights[10];
    };

    class LightningHandeler
    {
    public:
        LightningHandeler();
        ~LightningHandeler();

        Vector<SkyLight> m_SkyLights;
        Vector<PointLight> m_PointLights;

    public:
        void CalculateAndRenderLights(const glm::vec3& cameraPos, Ref<Material>& material);
        void ClearLights();

    private:
        Ref<ConstantBuffer> m_LightConstantBuffer;
        LightCBuffer m_LightCBufferData;
    };
}