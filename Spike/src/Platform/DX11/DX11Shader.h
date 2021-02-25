/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Shader
File Type      : h
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
#pragma once
#include "Spike/Renderer/Shader.h"
#include <d3d11.h>
#include <d3d11shader.h>
#include <d3d11shadertracing.h>

namespace Spike
{
    class DX11Shader : public Shader
    {
    public:
        DX11Shader(const String& filepath);
        DX11Shader(const String& source, const char* name);
        virtual ~DX11Shader();
        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void* GetNativeClass() override;
        virtual RendererID GetRendererID() const override { return (RendererID)nullptr; } //TODO
        virtual const String& GetName() const override { return m_Name; }
        virtual String GetFilepath() const override { return m_Filepath; };

        //TODO: Remove
        virtual void SetInt(const String& name, int value) override {};
        virtual void SetIntArray(const String& name, int* value, uint32_t count) override {};
        virtual void SetFloat(const String& name, float value) override {};
        virtual void SetFloat2(const String& name, const glm::vec2& value) override {};
        virtual void SetFloat3(const String& name, const glm::vec3& value) override {};
        virtual void SetFloat4(const String& name, const glm::vec4& value) override {};
        virtual void SetMat4(const String& name, const glm::mat4& value) override {};
    public:
        ID3DBlob* GetVSRaw() { return m_RawBlobs.at(D3D11_VERTEX_SHADER); }
        ID3DBlob* GetPSRaw() { return m_RawBlobs.at(D3D11_PIXEL_SHADER); }

    private:
        std::unordered_map<D3D11_SHADER_TYPE, String> PreProcess(const String& source);
        void Compile();

    private:
        ID3D11VertexShader* m_VertexShader = nullptr;
        ID3D11PixelShader*  m_PixelShader  = nullptr;
        std::unordered_map<D3D11_SHADER_TYPE, ID3DBlob*> m_RawBlobs;
        std::unordered_map<D3D11_SHADER_TYPE, String> m_ShaderSources;

        String m_Name; //With Extension
        String m_Filepath;
    };
}
