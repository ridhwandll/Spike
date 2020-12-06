#pragma once
#include "LightEngine.h"
#include "Panels/SceneHierarchyPanel.h"

namespace LightEngine
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
        void OpenScene();
        void SaveSceneAs();
    private:
        OrthographicCameraController m_CameraController;
        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_FlatColorShader;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;

        Ref<Texture2D> m_CheckerboardTexture;

        Entity m_SquareEntity;
        Entity m_SecondSquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;

        bool m_PrimaryCamera = true;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = {0.0f, 0.0f};
        int m_GizmoType = -1;
        //Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };

}