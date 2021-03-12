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

        virtual Uint GetCount() const = 0;
        static Ref<IndexBuffer> Create(void* indices, Uint size);
    };
}