//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/IndexBuffer.h"

namespace Spike
{
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(void* indices, Uint count);
        virtual ~OpenGLIndexBuffer();
    
        virtual void Bind() const;
        virtual void Unbind() const;
    
        virtual Uint GetCount() const { return m_Count; }
    private:
        RendererID m_RendererID;
        Uint m_Count;
    };
}
