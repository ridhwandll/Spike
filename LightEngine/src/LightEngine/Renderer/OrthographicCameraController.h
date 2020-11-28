#pragma once
#include "LightEngine/Renderer/OrthographicCamera.h"
#include "LightEngine/Core/Timestep.h"
#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/Events/MouseEvent.h"

namespace LightEngine
{
    struct OrthographicCameraBounds
    {
        float Left, Right;
        float Bottom, Top;

        float GetWidth() { return Right - Left; }
        float GetHeight() { return Top - Bottom; }
    };

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRation, bool rotation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

        float GetCameraSpeed() { return m_CameraTranslationSpeed; }
        void SetCameraSpeed(float speed) { m_CameraTranslationSpeed = speed; }

        float GetCameraRotationSpeed() { return m_CameraRotationSpeed; }
        void SetCameraRotationSpeed(float speed) { m_CameraRotationSpeed = speed; }

        const OrthographicCameraBounds& GetBounds() { return m_Bounds; }
        float GetZoomLevel() { return m_ZoomLevel; }
        void SetZoomLevel(float level) { m_ZoomLevel = level; }
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        bool m_Rotation = false;
        OrthographicCameraBounds m_Bounds;
        OrthographicCamera m_Camera;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
    };
}