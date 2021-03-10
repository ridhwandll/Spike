//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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
        OpenGLShader(const String& source, const char* name);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual String GetFilepath() const override { return m_Filepath; }
        virtual RendererID GetRendererID() const override { return m_RendererID; }
        const String& GetName() const override { return m_Name; }
        virtual void* GetNativeClass() override;

        virtual void SetInt(const String& name, int value) override;
        virtual void SetIntArray(const String& name, int* value, uint32_t count) override;
        virtual void SetFloat(const String& name, float value) override;
        virtual void SetFloat2(const String& name, const glm::vec2& value) override;
        virtual void SetFloat3(const String& name, const glm::vec3& value) override;
        virtual void SetFloat4(const String& name, const glm::vec4& value) override;
        virtual void SetMat4(const String& name, const glm::mat4& value) override;
    private:
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
        String m_Name, m_Filepath;
        RendererID m_RendererID;
        std::unordered_map<GLenum, String> m_ShaderSource;
    };

}