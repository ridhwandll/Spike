/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ImGuiLayer
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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "ImGuiLayer.h"
#include "Spike/Renderer/RendererAPISwitch.h"
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "Spike/Core/Application.h"
#include <GLFW/glfw3.h> // TEMPORARY
#include <glad/glad.h> // TEMPORARY
#include <ImGuizmo.h>
#include <FontAwesome.h>

#ifdef RENDERER_API_OPENGL
    #include "backends/imgui_impl_opengl3.h"
#elif defined RENDERER_API_DX11
    #include "Platform/DX11/DX11Internal.h"
    #include "backends/imgui_impl_dx11.h"
#endif

namespace Spike
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    void ImGuiLayer::OnAttach()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        io.Fonts->AddFontFromFileTTF("Spike-Editor/assets/fonts/Ruda/Ruda-ExtraBold.ttf", 15.0f);             //Font index 0
        io.Fonts->AddFontFromFileTTF("Spike-Editor/assets/fonts/JetBrains/JetBrainsMono-Medium.ttf", 15.0f);  //Font index 1
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Spike-Editor/assets/fonts/Ruda/Ruda-Regular.ttf", 15.0f);

        ImFontConfig config;
        config.MergeMode = true;
        static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
        io.Fonts->AddFontFromFileTTF("Spike-Editor/assets/fonts/fontawesome-webfont.ttf", 14.0f, &config, icon_ranges);
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }   
        SetDarkThemeColors();
        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    #ifdef RENDERER_API_OPENGL
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    #elif defined RENDERER_API_DX11
        ImGui_ImplGlfw_InitForVulkan(window, true); //GLFW init
        ImGui_ImplDX11_Init(DX11Internal::GetDevice(), DX11Internal::GetDeviceContext());
    #endif
    }
    void ImGuiLayer::OnEvent(Event& e)
    {
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }
    void ImGuiLayer::OnDetach()
    {
    #ifdef RENDERER_API_OPENGL
        ImGui_ImplOpenGL3_Shutdown();
    #elif defined RENDERER_API_DX11
        ImGui_ImplDX11_Shutdown();
    #endif
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin()
    {
    #ifdef RENDERER_API_OPENGL
        ImGui_ImplOpenGL3_NewFrame();
    #elif defined RENDERER_API_DX11
        ImGui_ImplDX11_NewFrame();
    #endif
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
    #ifdef RENDERER_API_OPENGL
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #elif defined RENDERER_API_DX11
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    #endif

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        auto& style = ImGui::GetStyle();
        auto& colors = ImGui::GetStyle().Colors;

        style.TabRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] =             ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] =      ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] =       ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Checbox
        colors[ImGuiCol_CheckMark] =          ImVec4{ 0.0f, 0.8f, 0.0f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] =             ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] =      ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] =       ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] =            ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] =     ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] =      ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] =                ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] =         ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] =          ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] =       ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] =            ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] =      ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] =   ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

}