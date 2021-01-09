/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : OrthographicCameraController
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/OrthographicCamera.h"
#include "Spike/Core/Timestep.h"
#include "Spike/Events/ApplicationEvent.h"
#include "Spike/Events/MouseEvent.h"

namespace Spike
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
        void OnResize(float width, float height);

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