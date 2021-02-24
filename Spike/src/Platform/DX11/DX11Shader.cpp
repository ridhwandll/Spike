/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Shader
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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "DX11Shader.h"
#include "DX11Internal.h"
#include "Spike/Core/Vault.h"
#include <d3dcompiler.h>

namespace Spike
{
    static D3D11_SHADER_TYPE ShaderTypeFromString(const String& type)
    {
        if (type == "vertex")
            return D3D11_VERTEX_SHADER;
        if (type == "pixel" || type == "fragment")
            return D3D11_PIXEL_SHADER;

        SPK_CORE_ASSERT(false, "Unknown shader type!", type);
        return static_cast<D3D11_SHADER_TYPE>(0);
    }

    static String& ShaderVersionFromType(const D3D11_SHADER_TYPE type)
    {
        static String errorString = "No valid conversion found to DX11 shader version from DX11 type ";
        static String vertexVersion = "vs_5_0";
        static String pixelVersion = "ps_5_0";

        switch (type)
        {
            case D3D11_VERTEX_SHADER: return vertexVersion;
            case D3D11_PIXEL_SHADER: return pixelVersion;
        }

        SPK_CORE_ASSERT(false, "Unknown shader type!", type);
        return errorString;
    }

    DX11Shader::DX11Shader(const String& filepath)
        :m_Filepath(filepath)
    {
        m_Name = Vault::GetNameWithExtension(filepath);
        String source = Vault::ReadFile(filepath);
        m_ShaderSources = PreProcess(source);
        Compile();

        auto device = DX11Internal::GetDevice();
        for (auto& kv : m_RawBlobs)
        {
            switch (kv.first)
            {
                case D3D11_VERTEX_SHADER:
                    DX_CALL(device->CreateVertexShader(kv.second->GetBufferPointer(), kv.second->GetBufferSize(), NULL, &m_VertexShader));
                    break;
                case D3D11_PIXEL_SHADER:
                    DX_CALL(device->CreatePixelShader(kv.second->GetBufferPointer(), kv.second->GetBufferSize(), NULL, &m_PixelShader));
                    break;
            }
        }
    }

    DX11Shader::DX11Shader(const String& source, const char* name)
    {
        m_Name = name;
        m_ShaderSources = PreProcess(source);
        Compile();

        auto device = DX11Internal::GetDevice();
        for (auto& kv : m_RawBlobs)
        {
            switch (kv.first)
            {
                case D3D11_VERTEX_SHADER:
                    DX_CALL(device->CreateVertexShader(kv.second->GetBufferPointer(), kv.second->GetBufferSize(), NULL, &m_VertexShader));
                    break;
                case D3D11_PIXEL_SHADER:
                    DX_CALL(device->CreatePixelShader(kv.second->GetBufferPointer(), kv.second->GetBufferSize(), NULL, &m_PixelShader));
                    break;
            }
        }
    }

    void DX11Shader::Bind() const
    {
        auto deviceContext = DX11Internal::GetDeviceContext();
        for (auto& kv : m_RawBlobs)
        {
            switch (kv.first)
            {
                case D3D11_VERTEX_SHADER:
                    deviceContext->VSSetShader(m_VertexShader, nullptr, 0);
                    break;
                case D3D11_PIXEL_SHADER:
                    deviceContext->PSSetShader(m_PixelShader, nullptr, 0);
                    break;
            }
        }
    }

    void DX11Shader::Unbind() const 
    {
        auto deviceContext = DX11Internal::GetDeviceContext();
        for (auto& kv : m_RawBlobs)
        {
            switch (kv.first)
            {
                case D3D11_VERTEX_SHADER:
                    deviceContext->VSSetShader(nullptr, 0, 0);
                    break;
                case D3D11_PIXEL_SHADER:
                    deviceContext->PSSetShader(nullptr, 0, 0);
                    break;
            }
        }
    }

    void* DX11Shader::GetNativeClass()
    {
        return this;
    }

    std::unordered_map<D3D11_SHADER_TYPE, String> DX11Shader::PreProcess(const String& source)
    {
        std::unordered_map<D3D11_SHADER_TYPE, String> shaderSources;
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line

        while (pos != String::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            SPK_CORE_ASSERT(eol != String::npos, "Syntax error");
            size_t being = pos + typeTokenLength + 1; //Start of shader type name(after "#type " keyword)
            String type = source.substr(being, eol - being);
            SPK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            SPK_CORE_ASSERT(nextLinePos != String::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line
            shaderSources[ShaderTypeFromString(type)] = (pos == String::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }
        return shaderSources;
    }

    void DX11Shader::Compile()
    {
        HRESULT result;
        ID3DBlob* errorRaw = nullptr;
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        #if defined SPK_DEBUG
            flags |= D3DCOMPILE_DEBUG;
        #endif

        for (auto& kv : m_ShaderSources)
        {
            D3D11_SHADER_TYPE type = kv.first;
            const String& source = kv.second;

            //https://docs.microsoft.com/en-us/windows/win32/api/d3dcompiler/nf-d3dcompiler-d3dcompile
            result = D3DCompile(source.c_str(), source.size(), NULL, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", ShaderVersionFromType(type).c_str(), flags, 0, &m_RawBlobs[type], &errorRaw);

            if (FAILED(result))
            {
                char* errorText = (char*)errorRaw->GetBufferPointer();
                errorText[strlen(errorText) - 1] = '\0';

                SPK_CORE_LOG_ERROR("%s", errorText);
                SPK_CORE_ASSERT(false, "Shader compilation failure!");
                errorRaw->Release();
                return;
            }
            if (errorRaw) errorRaw->Release();
        }
    }

    DX11Shader::~DX11Shader()
    {
        for (auto& kv : m_RawBlobs)
            kv.second->Release();
        m_RawBlobs.clear();
        if(m_VertexShader) m_VertexShader->Release();
        if(m_PixelShader) m_PixelShader->Release();
    }
}