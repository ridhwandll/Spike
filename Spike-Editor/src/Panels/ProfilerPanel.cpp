//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "ProfilerPanel.h"
#include "Spike/Core/Application.h"
#include "Spike/Renderer/Renderer.h"
#include "Spike/Renderer/Renderer2D.h"
#include "UIUtils/UIUtils.h"
#include <imgui/imgui.h>

namespace Spike
{
    void ProfilerPanel::OnImGuiRender()
    {
        float avg = 0.0f;
        const uint32_t size = m_FrameTimes.size();
        if (size >= 50)
            m_FrameTimes.erase(m_FrameTimes.begin());

        m_FrameTimes.push_back(ImGui::GetIO().Framerate);
        for (uint32_t i = 0; i < size; i++)
        {
            m_FPSValues[i] = m_FrameTimes[i];
            avg += m_FrameTimes[i];
        }
        avg /= size;

        ImGui::Begin("Profiler");
        auto& caps = RendererAPI::GetCapabilities();
        ImGui::Text("Vendor: %s", caps.Vendor.c_str());
        ImGui::Text("Renderer: %s", caps.Renderer.c_str());
        const float fps = (1.0f / avg) * 1000.0f;
        ImGui::Text("Frame time (ms): %f", fps);

        ImGui::Text("FPS: %f", avg);
        ImGui::SameLine();
        ImGui::Columns(2);
        ImGui::NextColumn();
        ImGui::SetColumnWidth(0, 130.0f);
        ImGui::PlotHistogram("##FPS", m_FPSValues, size);
        ImGui::Columns(1);
        m_VSync = Application::Get().GetWindow().IsVSync();
        if (GUI::DrawBoolControl("VSync Enabled", &m_VSync, 130.0f))
            Application::Get().GetWindow().SetVSync(m_VSync);
        ImGui::Separator();
        ImGui::Text("Renderer");
        ImGui::Text("Draw Calls: %d", Renderer::GetTotalDrawCallsCount());
        ImGui::Separator();
        auto& stats2D = Renderer2D::GetStats();
        ImGui::Text("Renderer2D");
        ImGui::Text("Draw Calls: %d", stats2D.DrawCalls);
        ImGui::Text("Quad Count: %d", stats2D.QuadCount);
        ImGui::Text("Vertices: %d", stats2D.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats2D.GetTotalIndexCount());
        ImGui::End();
    }

}