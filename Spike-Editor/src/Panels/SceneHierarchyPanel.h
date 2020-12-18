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
#include "Panel.h"
#include "Spike/Core/Log.h"
#include "Spike/Core/Base.h"
#include "Spike/Scene/Entity.h"
#include "Spike/Events/Event.h"
#include "Spike/Events/KeyEvent.h"

namespace Spike
{
    class SceneHierarchyPanel : public Panel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);
        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();
        void OnEvent(Event& e);
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        void DrawEntityNode(Entity entity);
    private:
        bool m_IsHierarchyFocused = false;
        bool m_IsHierarchyHovered = false;
    };
}