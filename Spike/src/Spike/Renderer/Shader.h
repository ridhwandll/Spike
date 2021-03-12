//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Ref.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Spike
{
    enum class ShaderDomain
    {
        NONE = 0,
        VERTEX,
        PIXEL
    };
    class Shader : public RefCounted
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual const String& GetName() const = 0;
        virtual String GetFilepath() const = 0;
        virtual RendererID GetRendererID() const = 0;
        virtual void* GetNativeClass() = 0;

        static Ref<Shader> Create(const String& filepath);

        virtual void SetInt(const String& name, int value) = 0;
        virtual void SetIntArray(const String& name, int* value, Uint count) = 0;
        virtual void SetFloat(const String& name, float value) = 0;
        virtual void SetFloat2(const String& name, const glm::vec2& value) = 0;
        virtual void SetFloat3(const String& name, const glm::vec3& value) = 0;
        virtual void SetFloat4(const String& name, const glm::vec4& value) = 0;
        virtual void SetMat4(const String& name, const glm::mat4& value) = 0;
    };
}