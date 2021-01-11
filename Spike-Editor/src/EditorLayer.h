/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : EditorLayer
File Type      : h
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
#pragma once
#include "Spike.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Spike/Renderer/EditorCamera.h"

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
        //bool OnMouseButtonPressed(MouseButtonPressedEvent& e); //Uncomment for MousePicking (Disabled because it crashes the engine sometimes)

        void NewScene();
        void OpenScene();
        void SaveSceneAs();
        void SaveScene();
    private:
        OrthographicCameraController m_CameraController;
        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_FlatColorShader;
        Ref<Framebuffer> m_Framebuffer;
        //Ref<Framebuffer> m_IDFramebuffer; //Uncomment for MousePicking (Disabled because it crashes the engine sometimes)

        Ref<Scene> m_ActiveScene;

        Ref<Texture2D> m_CheckerboardTexture;

        Entity m_SquareEntity;
        Entity m_SecondSquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;
        //Entity m_HoveredEntity; //Uncomment for MousePicking (Disabled because it crashes the engine sometimes)

        bool m_PrimaryCamera = true;
        EditorCamera m_EditorCamera;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = {0.0f, 0.0f};
        glm::vec2 m_ViewportBounds[2];

        std::string m_ActiveFilepath = std::string();
        bool m_FirstTimeSave = false;

        int m_GizmoType = -1;
        bool m_GizmoInUse = false;
        float m_FrameTime = 0.0f;
        //Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };

}