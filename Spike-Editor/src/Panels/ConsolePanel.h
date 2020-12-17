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
#include <vector>
#include <string>
#include "../Panels/Panel.h"
#include "../FontAwesome.h"
#include <imgui/imgui.h>

namespace Spike
{
    class Console : public Panel
    {
    public:
        ~Console();

        enum class LogLevel
        {
            LVL_INFO, LVL_DEBUG, LVL_WARN, LVL_ERROR, LVL_CRITICAL
        };

        static Console* Get();

        void OnImGuiRender();
        void Print(std::string message, LogLevel level = LogLevel::LVL_DEBUG);

        virtual Entity GetSelectedEntity() const override { return m_SelectionContext; }
        virtual Ref<Scene> GetCurrentScene() const override { return m_Context; }
    private:
        Console();
        void ClearLog();
    private:
        static Console* m_Console;
        std::vector<std::pair<LogLevel, std::string>> m_Messages{};
        bool m_ScrollLockEnabled = false;

        //Colors
        ImVec4 m_InfoColor = { 0.0f, 1.0f, 0.0f, 1.0f };
        ImVec4 m_DebugColor = { 0.0f, 0.5f, 1.0f, 1.0f };
        ImVec4 m_WarnColor = { 1.0f, 0.9f, 0.0f, 1.0f };
        ImVec4 m_ErrorColor = { 1.0f, 0.2f, 0.1f, 1.0f };
        ImVec4 m_CriticalColor = { 0.5f, 0.0f, 0.7f, 1.0f };

        //Bool switches
        bool m_WarningEnabled = true;
        bool m_InfoEnabled = true;
        bool m_DebugEnabled = true;
        bool m_ErrorEnabled = true;
    };
}