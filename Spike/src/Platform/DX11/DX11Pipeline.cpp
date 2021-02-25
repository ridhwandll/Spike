/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Pipeline
File Type      : cpp
File created on: 2021/02/12
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
#include "spkpch.h"
#include "DX11Pipeline.h"
#include "DX11Shader.h"
#include "DX11Internal.h"

namespace Spike
{
    static DXGI_FORMAT ShaderDataTypeToDirectXBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return DXGI_FORMAT_R32_FLOAT;
            case ShaderDataType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
            case ShaderDataType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
            case ShaderDataType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case ShaderDataType::Mat3:   return DXGI_FORMAT_R32_FLOAT;
            case ShaderDataType::Mat4:   return DXGI_FORMAT_R32_FLOAT;
            case ShaderDataType::Int:    return DXGI_FORMAT_R32_SINT;
            case ShaderDataType::Int2:   return DXGI_FORMAT_R32G32_SINT;
            case ShaderDataType::Int3:   return DXGI_FORMAT_R32G32B32_SINT;
            case ShaderDataType::Int4:   return DXGI_FORMAT_R32G32B32A32_SINT;
            case ShaderDataType::Bool:   SPK_CORE_ASSERT(false, "Shader data type bool is not supported!");
        };
        SPK_CORE_ASSERT(false, "There is no DirectX base type for given shader data type.");
        return DXGI_FORMAT_R32_FLOAT;
    }

    static D3D_PRIMITIVE_TOPOLOGY SpikeTopologyToDX11Topology(PrimitiveTopology topology)
    {
        switch (topology)
        {
            case PrimitiveTopology::UNDEFINED:     return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
            case PrimitiveTopology::POINTLIST:     return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            case PrimitiveTopology::LINELIST:      return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            case PrimitiveTopology::LINESTRIP:     return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
            case PrimitiveTopology::TRIANGLELIST:  return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case PrimitiveTopology::TRIANGLESTRIP: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        }
        return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }

    DX11Pipeline::DX11Pipeline(const PipelineSpecification& spec)
        :m_Spec(spec)
    {
        auto& elements = m_Spec.VertexBuffer->GetLayout().GetElements();
        D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[elements.size()];

        for (int i = 0; i < elements.size(); i++)
        {
            auto& element = elements[i];
            ied[i] =
            {
                element.Name.c_str(), 0, ShaderDataTypeToDirectXBaseType(element.Type), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
            };
        }

        DX11Shader* nativeShader = static_cast<DX11Shader*>(m_Spec.Shader->GetNativeClass());
        DX11Internal::GetDevice();
        DX_CALL(DX11Internal::GetDevice()->CreateInputLayout(
            ied,
            (UINT)elements.size(),
            static_cast<ID3DBlob*>(nativeShader->GetVSRaw())->GetBufferPointer(),
            static_cast<ID3DBlob*>(nativeShader->GetVSRaw())->GetBufferSize(),
            &m_InputLayout));
        delete[] ied;
    }

    DX11Pipeline::~DX11Pipeline()
    {
        if (m_InputLayout)
            m_InputLayout->Release();
    }

    void DX11Pipeline::Bind() const
    {
        DX11Internal::GetDeviceContext()->IASetPrimitiveTopology(SpikeTopologyToDX11Topology(m_PrimitiveTopology));
        DX11Internal::GetDeviceContext()->IASetInputLayout(m_InputLayout);
    }

    void DX11Pipeline::Unbind() const
    {

    }
}