//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike.h"
#include "Spike/Renderer/EditorCamera.h"
#include "Panels/ConsolePanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ProfilerPanel.h"
#include "Panels/VaultPanel.h"
#include "Panels/MaterialPanel.h"

namespace Spike
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Event& e) override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e);

        void NewScene();
        void OpenFolder();
        void OpenScene();
        void SaveScene();
        void SaveSceneAs();
        void UpdateWindowTitle(const String& sceneName);
        void DrawRectAroundWindow(const glm::vec4& color);
        void RenderGizmos();
        void RenderPanels();

        void OnScenePlay();
        void OnSceneStop();
        void OnScenePause();
        void OnSceneResume();
    private:
        enum class SceneState
        {
            Edit = 0, Play = 1, Pause = 2
        };
        SceneState m_SceneState = SceneState::Edit;

        Ref<Framebuffer> m_Framebuffer;
        Ref<Scene> m_EditorScene, m_RuntimeScene;
        EditorCamera m_EditorCamera;
        Entity m_StartupCameraEntity;
        Entity m_SelectedEntity;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        bool m_FirstTimeSave = false;
        bool m_ReloadScriptOnPlay = true;
        String m_ActiveFilepath = String();
        String m_CurrentSkyboxPath = String();
        int m_GizmoType = -1;
        bool m_GizmoInUse = false;

        //Panel(s)
        SceneHierarchyPanel m_SceneHierarchyPanel;
        ProfilerPanel m_ProfilerPanel;
        VaultPanel m_VaultPanel;
        MaterialPanel m_MaterialPanel;
    private:
        friend class VaultPanel;
    };

}