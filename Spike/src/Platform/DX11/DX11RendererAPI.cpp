//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#include "spkpch.h"
#include "DX11RendererAPI.h"
#include "Spike/Core/Application.h"
#include "DX11Internal.h"
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Spike
{
    void DX11RendererAPI::Init()
    {
    }

    void DX11RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        DX11Internal::Resize(width, height);
    }

    void DX11RendererAPI::SetClearColor(const glm::vec4& color)
    {
        m_ClearColor = color;
    }

    void DX11RendererAPI::Clear()
    {
        DX11Internal::GetBackbuffer()->Clear(m_ClearColor);
    }

    void DX11RendererAPI::DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t indexCount)
    {
        uint32_t count = indexCount ? indexCount : pipeline->GetSpecification().IndexBuffer->GetCount();
        DX11Internal::GetDeviceContext()->DrawIndexed(count, 0, 0);
    }

    void DX11RendererAPI::DrawIndexedMesh(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex)
    {
        DX11Internal::GetDeviceContext()->DrawIndexed(indexCount, baseIndex, baseVertex);
    }

    void DX11RendererAPI::BindBackbuffer()
    {
        DX11Internal::BindBackbuffer();
    }

    void DX11RendererAPI::BeginWireframe()
    {
        DX11Internal::BeginWireframe();
    }

    void DX11RendererAPI::EndWireframe()
    {
        DX11Internal::EndWireframe();
    }
}