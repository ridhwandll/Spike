/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--
         Copyright 2021 - SpikeTechnologies - All Rights Reserved
File Name      : OpenGLShader
File Type      : cpp
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
#include "spkpch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Panels/ConsolePanel.h"

namespace Spike
{
    static GLenum ShaderTypeFromString(const String& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        SPK_INTERNAL_ASSERT("Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const String& filepath)
        :m_Filepath(filepath)
    {
        // Extract name from the filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == String::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == String::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);
        Reload();
    }

    void OpenGLShader::Reload()
    {
        std::string source = ReadFile(m_Filepath);
        Load(source);
    }

    void OpenGLShader::Load(const std::string& source)
    {
        m_ShaderSource = PreProcess(source);

        if (m_RendererID)
            glDeleteProgram(m_RendererID);

        Compile();
        if (m_Loaded)
        {
            for (auto& callback : m_ShaderReloadedCallbacks)
                callback();
        }

        m_Loaded = true;
    }

    void OpenGLShader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
    {
        m_ShaderReloadedCallbacks.push_back(callback);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    String OpenGLShader::ReadFile(const String& filepath)
    {
        String result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
        }
        else
        {
            SPK_CORE_LOG_CRITICAL("Could not open file path \"{0}\"", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, String> OpenGLShader::PreProcess(const String& source)
    {
        std::unordered_map<GLenum, String> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != String::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            SPK_CORE_ASSERT(eol != String::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            String type = source.substr(begin, eol - begin);
            SPK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            SPK_CORE_ASSERT(nextLinePos != String::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[ShaderTypeFromString(type)] = (pos == String::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile()
    {
        GLuint program = glCreateProgram();
        SPK_CORE_ASSERT(shaderSources.size() <= 2, "We only support two shaders for now.");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for (auto& keyvalue : m_ShaderSource)
        {
            GLenum type = keyvalue.first;
            const String& source = keyvalue.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCstr = source.c_str();
            glShaderSource(shader, 1, &sourceCstr, 0);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                glDeleteShader(shader);

                SPK_CORE_LOG_CRITICAL("{0}", infoLog.data());
                SPK_INTERNAL_ASSERT("Shader compilation failure!");
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            for (auto id : glShaderIDs)
                glDeleteShader(id);
            SPK_CORE_LOG_CRITICAL("{0}", infoLog.data());
            SPK_INTERNAL_ASSERT("Shader link failure!");
        }
        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_RendererID = program;
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt       (const String& name, int value)                  { UploadUniformInt     (name, value);        }
    void OpenGLShader::SetIntArray  (const String& name, int* value, uint32_t count) { UploadUniformIntArray(name, value, count); }
    void OpenGLShader::SetFloat     (const String& name, float value)                { UploadUniformFloat   (name, value);        }
    void OpenGLShader::SetFloat2    (const String& name, const glm::vec2& value)     { UploadUniformFloat2  (name, value);        }
    void OpenGLShader::SetFloat3    (const String& name, const glm::vec3& value)     { UploadUniformFloat3  (name, value);        }
    void OpenGLShader::SetFloat4    (const String& name, const glm::vec4& value)     { UploadUniformFloat4  (name, value);        }
    void OpenGLShader::SetMat4      (const String& name, const glm::mat4& value)     { UploadUniformMat4    (name, value);        }

    void OpenGLShader::UploadUniformInt(const String& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const String& name, int* value, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniform1iv(location, count, value);
    }

    void OpenGLShader::UploadUniformFloat(const String& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const String& name, const glm::vec2& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::UploadUniformFloat3(const String& name, const glm::vec3& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::UploadUniformFloat4(const String& name, const glm::vec4& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadUniformMat3(const String& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const String& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1)
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::DumpShaderData()
    {
        auto console = Console::Get();

        GLint i;
        GLint count;

        GLint size; // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)

        const GLsizei bufSize = 50; // maximum name length
        GLchar name[bufSize]; // variable name in GLSL
        GLsizei length; // name length

        glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTES, &count);
        SPK_LOG_INFO("==========SPIKE-ENGINE==========");
        SPK_LOG_INFO("========== {0} ==========", this->GetName());
        SPK_LOG_INFO("====ATTRIBUTES====");
        SPK_LOG_INFO("Active Attributes: {0}", count);

        console->Print("==========SPIKE-ENGINE==========");
        console->Print("========== " + this->GetName() + " ==========");
        console->Print("====ATTRIBUTES====");
        console->Print("Active Attributes " + std::to_string(count));

        for (i = 0; i < count; i++)
        {
            glGetActiveAttrib(m_RendererID, (GLuint)i, bufSize, &length, &size, &type, name);
            SPK_LOG_INFO("Attribute {0} Type: {1} Name: {2}", i, type, name);
            console->Print("Attribute " + std::to_string(i) + " Type: " + std::to_string(type) + " Name: " + name);
        }

        glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &count);
        SPK_LOG_INFO("====UNIFORMS====");
        SPK_LOG_INFO("Active Uniforms: {0}", count);

        console->Print("====UNIFORMS====");
        console->Print("Active Uniforms: " + std::to_string(count));

        for (i = 0; i < count; i++)
        {
            glGetActiveUniform(m_RendererID, (GLuint)i, bufSize, &length, &size, &type, name);
            SPK_LOG_INFO("Uniform {0} Type: {1} Name: {2}", i, type, name);
            console->Print("Uniform " + std::to_string(i) + " Type: " + std::to_string(type) + " Name: " + name);
        }
    }
}