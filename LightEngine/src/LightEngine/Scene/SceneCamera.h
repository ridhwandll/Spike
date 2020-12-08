/*****************************************************************************/
/*                        Lightengine  SourceCode                            */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: None                                                       */
/*                                                                           */
/*   Licensed under the Apache License, Version 2.0 (the "License");         */
/*   you may not use this file except in compliance with the License.        */
/*   You may obtain a copy of the License at                                 */
/*                                                                           */
/*       http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                           */
/*   Unless required by applicable law or agreed to in writing, software     */
/*   distributed under the License is distributed on an "AS IS" BASIS,       */
/*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*   See the License for the specific language governing permissions and     */
/*   limitations under the License.                                          */
/*****************************************************************************/
#pragma once
#include "LightEngine/Renderer/Camera.h"

namespace LightEngine
{
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetPerspective(float verticalFOV, float nearClip, float farClip);
        void SetOrthographic(float size, float nearClip, float farClip);

        void SetViewportSize(uint32_t width, uint32_t height);

        //Perspective
        float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
        void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }

        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }

        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }


        //Orthographic
        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }

        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
    private:
        void RecalculateProjection();
    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;

        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;
    };

}
