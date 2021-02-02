/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--
         Copyright 2021 - SpikeTechnologies - All Rights Reserved
File Name      : OpenGLShader
File Type      : h
File created on: 2021/01/09
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
#pragma once
#include <Spike/Renderer/Shader.h>
#include <glm/glm.hpp>

typedef unsigned int GLenum;
namespace Spike
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const String& filepath);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetInt(const String& name, int value) override;
        virtual void SetIntArray(const String& name, int* value, uint32_t count) override;
        virtual void SetFloat(const String& name, float value) override;
        virtual void SetFloat2(const String& name, const glm::vec2& value) override;
        virtual void SetFloat3(const String& name, const glm::vec3& value) override;
        virtual void SetFloat4(const String& name, const glm::vec4& value) override;
        virtual void SetMat4(const String& name, const glm::mat4& value) override;

        const String& GetName() const override { return m_Name; }
        virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;
        virtual void Load(const std::string& source) override;
        virtual void DumpShaderData() override;

        void Reload();
    private:
        String ReadFile(const String& filepath);
        std::unordered_map<GLenum, String> PreProcess(const String& source);
        void Compile();

        void UploadUniformInt(const String& name, int value);
        void UploadUniformIntArray(const String& name, int* value, uint32_t count);
        void UploadUniformFloat(const String& name, float value);
        void UploadUniformFloat2(const String& name, const glm::vec2& matrix);
        void UploadUniformFloat3(const String& name, const glm::vec3& values);
        void UploadUniformFloat4(const String& name, const glm::vec4& values);
        void UploadUniformMat3(const String& name, const glm::mat3& matrix);
        void UploadUniformMat4(const String& name, const glm::mat4& matrix);
    private:
        bool m_Loaded = false;
        String m_Name, m_Filepath;
        RendererID m_RendererID;
        std::unordered_map<GLenum, String> m_ShaderSource;
        std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
    };

}