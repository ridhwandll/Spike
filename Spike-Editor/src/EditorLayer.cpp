/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : EditorLayer
File Type      : cpp
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "EditorLayer.h"
#include "Panels/ConsolePanel.h"
#include "Spike/Scene/SceneSerializer.h"
#include "Spike/Utility/PlatformUtils.h"
#include "Spike/Math/Math.h"
#include "Spike/Scripting/ScriptEngine.h"
#include "UIUtils/UIUtils.h"
#include <FontAwesome.h>
#include <imgui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(push)
#pragma warning(disable : 4244) //Disable ugly 'C4244' "type conversion" warning!

namespace Spike
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    void EditorLayer::OnAttach()
    {
        //Application::Get()->GetWindow().SetVSync(false);
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_EditorScene = Ref<Scene>::Create();
        m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
        m_SceneHierarchyPanel.SetContext(m_EditorScene);

        LaunchReadymadeScene();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnScenePlay()
    {
        if (m_ReloadScriptOnPlay)
        {
            ScriptEngine::SetSceneContext(m_EditorScene);
            ScriptEngine::ReloadAssembly("Spike-Editor/assets/scripts/ExampleApp.dll");
        }
        m_SceneHierarchyPanel.ClearSelectedEntity();
        m_SceneState = SceneState::Play;

        m_RuntimeScene = Ref<Scene>::Create();
        m_EditorScene->CopySceneTo(m_RuntimeScene);

        m_RuntimeScene->OnRuntimeStart();
        m_SceneHierarchyPanel.SetContext(m_RuntimeScene);
    }

    void EditorLayer::OnSceneStop()
    {
        m_RuntimeScene->OnRuntimeStop();
        m_SceneState = SceneState::Edit;

        // Unload runtime scene
        m_RuntimeScene = nullptr;

        m_SceneHierarchyPanel.ClearSelectedEntity();
        m_SceneHierarchyPanel.SetContext(m_EditorScene);
    }

    void EditorLayer::OnScenePause()
    {
        m_SceneState = SceneState::Pause;
    }

    void EditorLayer::OnSceneResume()
    {
        m_SceneState = SceneState::Play;
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        // Resize
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Render
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();
        m_Framebuffer->ClearAttachment(1, -1);

        switch (m_SceneState)
        {
        case EditorLayer::SceneState::Edit:
            {
                m_EditorCamera.OnUpdate(ts);
                m_EditorScene->OnUpdateEditor(ts, m_EditorCamera);
                break;
            }
        case EditorLayer::SceneState::Play:
            {
                if (m_ViewportFocused)
                    m_EditorCamera.OnUpdate(ts);

                m_RuntimeScene->OnUpdate(ts);
                m_RuntimeScene->OnUpdateRuntime(ts);
                break;
            }
        case EditorLayer::SceneState::Pause:
            {
                if (m_ViewportFocused)
                    m_EditorCamera.OnUpdate(ts);

                m_RuntimeScene->OnUpdateRuntime(ts);
                break;
            }
        }

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = m_ViewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_ViewportSize.x &&
            mouseY < (int)m_ViewportSize.y && Input::IsMouseButtonPressed(Mouse::Button0) && !ImGuizmo::IsUsing())
        {
            auto pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_SelectedEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_EditorScene.Raw());
        }
        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 270.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        Console::Get()->OnImGuiRender();
        ScriptEngine::OnImGuiRender();
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    SaveScene();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    SaveSceneAs();

                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Script"))
            {
                ImGui::MenuItem("Reload assembly on play", nullptr, &m_ReloadScriptOnPlay);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ProfilerPanel.OnImGuiRender(m_SelectedEntity);

        bool show = true;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5, 0.5, 0.5, 1.0f));
        ImGui::Begin("ToolBar", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
        if (ImGui::Button(ICON_FK_REPEAT))
        {
            ScriptEngine::ReloadAssembly("Spike-Editor/assets/scripts/ExampleApp.dll");
            Console::Get()->Print("ScriptEngine reloaded the C# assembly successfully!", Console::LogLevel::LVL_INFO);
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
            ImGui::TextUnformatted("Reload C#");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        ImGui::SameLine();

        if (ImGui::Button(ICON_FK_FLOPPY_O))
        {
            SaveScene();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
            ImGui::TextUnformatted("Save");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
        if (m_SceneState == SceneState::Edit)
        {
            if (ImGui::ArrowButton("Play", ImGuiDir_Right))
            {
                OnScenePlay();
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FK_PAUSE))
            {
                Console::Get()->Print("You can pause the scene only in Playmode!", Console::LogLevel::LVL_WARN);
            }
        }
        else if (m_SceneState == SceneState::Play)
        {
            if (ImGui::Button(ICON_FK_STOP))
            {
                OnSceneStop();
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FK_PAUSE))
            {
                OnScenePause();
            }
        }
        else if (m_SceneState == SceneState::Pause)
        {
            if (ImGui::Button(ICON_FK_STOP))
            {
                OnSceneStop();
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FK_PAUSE))
            {
                OnSceneResume();
            }
        }

        ImGui::End();
        ImGui::PopStyleColor(3);

        ImGui::Begin("Renderer");
        if (ImGui::TreeNode("Shaders"))
        {
            auto& shaders = Shader::s_AllShaders;
            for (auto& shader : shaders)
            {
                if (ImGui::TreeNode(shader->GetName().c_str()))
                {
                    if (ImGui::Button("Reload"))
                        shader->Reload();
                    ImGui::SameLine();
                    if (ImGui::Button("Dump Shader Data"))
                        shader->DumpShaderData();
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin(ICON_FK_GAMEPAD" Viewport");
        auto viewportOffset = ImGui::GetCursorPos();

        if (m_SceneState == SceneState::Play)
            DrawRectAroundWindow({ 1.0f, 1.0f, 0.0f, 1.0f });
        else if (m_SceneState == SceneState::Pause)
            DrawRectAroundWindow({ 0.0f, 0.0f, 1.0f, 1.0f });

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        DrawGizmos();
        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
        m_ViewportBounds[0] = { minBound.x, minBound.y };
        m_ViewportBounds[1] = { maxBound.x, maxBound.y };

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_SceneHierarchyPanel.OnEvent(e);

        if (m_SceneState == SceneState::Edit)
        {
            if (m_ViewportHovered)
                m_EditorCamera.OnEvent(e);
            m_EditorScene->OnEvent(e);
        }
        else if (m_SceneState == SceneState::Play)
        {
            m_RuntimeScene->OnEvent(e);
        }

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SPK_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(SPK_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        switch (e.GetKeyCode())
        {
            case Key::N:
            {
                if (control)
                    NewScene();

                break;
            }
            case Key::O:
            {
                if (control)
                    OpenScene();

                break;
            }
            case Key::S:
            {
                if (control && shift)
                    SaveSceneAs();
                if (control)
                    SaveScene();
                break;
            }

            // Gizmos
            case Key::Q:
                if (!m_GizmoInUse)
                {
                    m_GizmoType = -1;
                }
                break;
            case Key::W:
                if (!m_GizmoInUse)
                {
                    m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                }
                break;
            case Key::E:
                if (!m_GizmoInUse)
                {
                    m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                }
                break;
            case Key::R:
                if (!m_GizmoInUse)
                {
                    m_GizmoType = ImGuizmo::OPERATION::SCALE;
                }
                break;
            case Key::Escape:
                if (m_SceneState == SceneState::Play)
                {
                    m_SceneState = SceneState::Edit;
                }
        }
        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        m_SceneHierarchyPanel.SetSelectedEntity(m_SelectedEntity);
        return false;
    }

    void EditorLayer::NewScene()
    {
        m_EditorScene = Ref<Scene>::Create();
        m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_EditorScene);
        m_FirstTimeSave = true;
        m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
        UpdateWindowTitle("Untitled Scene");
        Console::Get()->Print("Successfully created new scene!", Console::LogLevel::LVL_INFO);
    }

    void EditorLayer::LaunchReadymadeScene()
    {
        NewScene();
        m_StartupCameraEntity = m_EditorScene->CreateEntity(ICON_FK_CAMERA" Main Camera");
        m_StartupCameraEntity.AddComponent<CameraComponent>();
        m_StartupCameraEntity.GetComponent<TransformComponent>().Translation.z = 0.9f;
    }

    void EditorLayer::OpenScene()
    {
        String filepath = FileDialogs::OpenFile("Spike Scene (*.spike)\0*.spike\0");
        if (!filepath.empty())
        {
            m_FirstTimeSave = false;
            m_ActiveFilepath = filepath;
            m_EditorScene = Ref<Scene>::Create();
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);

            SceneSerializer serializer(m_EditorScene);
            serializer.Deserialize(filepath);
            Console::Get()->Print("Succesfully deserialized scene!", Console::LogLevel::LVL_INFO);
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        String filepath = FileDialogs::SaveFile("Spike Scene (*.spike)\0*.spike\0");
        if (!filepath.empty())
        {
            m_FirstTimeSave = false;
            SceneSerializer serializer(m_EditorScene);
            serializer.Serialize(filepath);
            Console::Get()->Print("Scene serialized succesfully!", Console::LogLevel::LVL_INFO);
        }
    }

    void EditorLayer::SaveScene()
    {
        if (m_FirstTimeSave)
        {
            SaveSceneAs();
        }
        else
        {
            SceneSerializer serializer(m_EditorScene);
            serializer.Serialize(m_ActiveFilepath);
            Console::Get()->Print("Scene Saved!", Console::LogLevel::LVL_INFO);
        }
    }

    void EditorLayer::UpdateWindowTitle(const String& sceneName)
    {
        String title = "Spike |" + sceneName + "| " + Application::GetPlatformName() + " - " + Application::GetConfigurationName() + " <" + Application::CurrentGraphicsAPI() + "> ";
        Application::Get().GetWindow().SetTitle(title);
    }

    void EditorLayer::DrawRectAroundWindow(const glm::vec4& color)
    {
        ImVec2 windowMin = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 windowMax = { windowMin.x + windowSize.x, windowMin.y + windowSize.y };
        ImGui::GetForegroundDrawList()->AddRect(windowMin, windowMax, ImGui::ColorConvertFloat4ToU32(ImVec4(color.x, color.y, color.z, color.w)));
    }

    void EditorLayer::DrawGizmos()
    {
        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        /* [Spike] We are not rendering Gizmos in Play mode! Maybe expose this via a ImGui toggle button? TODO [Spike] */
        if (selectedEntity && m_GizmoType != -1 && m_SceneState != SceneState::Play)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            glm::mat4 cameraView, cameraProjection;

            //if (m_SceneState == SceneState::Play) //TODO
            //{
            //    auto cameraEntity = m_EditorScene->GetPrimaryCameraEntity();
            //    const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            //    cameraProjection = camera.GetProjection();
            //    cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
            //}

            if (m_SceneState == SceneState::Edit)
            {
                cameraProjection = m_EditorCamera.GetProjection();
                cameraView = m_EditorCamera.GetViewMatrix();
            }

            // Entity transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
            // Snap to 45 degrees for rotation
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                m_GizmoInUse = true;
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
            else
            {
                m_GizmoInUse = false;
            }
        }
    }

}
#pragma warning (pop) // Pop the warning
