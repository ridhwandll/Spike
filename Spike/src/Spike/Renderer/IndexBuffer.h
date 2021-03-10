//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Ref.h"

namespace Spike
{
    class IndexBuffer : public RefCounted
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;
        static Ref<IndexBuffer> Create(void* indices, uint32_t size);
    };
}