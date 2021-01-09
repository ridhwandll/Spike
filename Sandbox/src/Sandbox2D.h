/*****************************************************************************/
/*                             Spike SourceCode                              */
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
#include <Spike.h>
#include "Spike/Core/Base.h"
#include "Spike/Renderer/EditorCamera.h"
#include "Spike/Renderer/Mesh.h"

class Sandbox2D : public Spike::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Spike::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Spike::Event& e) override;
private:
    Spike::EditorCamera m_CameraController;

    // TODO: These are temporary
    Spike::Mesh m_Mesh;
    Spike::Ref<Spike::Shader> m_FlatColorShader;
    Spike::Ref<Spike::VertexArray> m_SquareVA;
    Spike::Ref < Spike::Texture2D > m_CheckerboardTexture;
    glm::vec4 m_SquareColor = { 0.9f, 0.2f, 0.1f, 1.0f };
};