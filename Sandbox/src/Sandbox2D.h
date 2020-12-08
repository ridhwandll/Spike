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
#include <LightEngine.h>
#include "LightEngine/Core/Base.h"


class Sandbox2D : public LightEngine::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(LightEngine::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(LightEngine::Event& e) override;
private:
    LightEngine::OrthographicCameraController m_CameraController;

    // TODO: These are temporary
    LightEngine::Ref<LightEngine::Shader> m_FlatColorShader;
    LightEngine::Ref<LightEngine::VertexArray> m_SquareVA;
    LightEngine::Ref < LightEngine::Texture2D > m_CheckerboardTexture;
    glm::vec4 m_SquareColor = { 0.9f, 0.2f, 0.1f, 1.0f };
};