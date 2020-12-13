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
#include "Console.h"
#include <imgui/imgui.h>

namespace Spike
{
    Console::Console()
    {
    }

    void Console::OnImGuiRender()
    {
        ImGui::Begin("Console");

        if (ImGui::Button("Clear"))
            ClearLog();

        ImGui::BeginChild("Console");

        ImVec4 color;
        std::string type;
        for (auto itr = m_Messages.begin(); itr != m_Messages.end(); ++itr)
        {
            if (itr->first == LogLevel::LVL_DEBUG)
            {
                color = { 0, 0.5, 1, 1 };
                type = m_IconDebug + " [DEBUG] ";
            }
            if (itr->first == LogLevel::LVL_INFO)
            {
                color = { 0, 1, 0, 1 };
                type = m_IconInfo + " [INFO] ";
            }
            if (itr->first == LogLevel::LVL_WARN)
            {
                color = { 1, 0.9, 0, 1 };
                type = m_IconWarn + " [WARNING] ";
            }
            if (itr->first == LogLevel::LVL_ERROR)
            {
                color = { 1, 0, 0, 1 };
                type = m_IconError + " [ERROR] ";
            }
            ImGui::TextColored(color, (type + itr->second).c_str());
            ImGui::Separator();
        }
        ImGui::EndChild();

        ImGui::End();
    }

    void Console::Print(std::string message, LogLevel level)
    {
        switch (level)
        {
            case Spike::Console::LogLevel::LVL_INFO:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_INFO, message));
                break;
            }
            case Spike::Console::LogLevel::LVL_DEBUG:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_DEBUG, message));
                break;
            }
            case Spike::Console::LogLevel::LVL_WARN:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_WARN, message));
                break;
            }
            case Spike::Console::LogLevel::LVL_ERROR:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_ERROR, message));
                break;
            }
        }
    }

    void Console::ClearLog()
    {
        m_Messages.clear();
    }

}