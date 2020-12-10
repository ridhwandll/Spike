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
#include "Console.h"
#include <imgui/imgui.h>
#include <chrono>


namespace LightEngine
{
    Console::Console()
    {
    }

    void Console::OnImGuiRender()
    {
        ImGui::Begin("Console");

        if (ImGui::Button("Clear"))
            ClearLog();

        for (auto itr = m_Messages.begin(); itr != m_Messages.end(); ++itr)
        {
            if (itr->first == LogLevel::LVL_DEBUG)
            {
                ImGui::TextColored({ 0, 0.5, 1, 1 }, ("[DEBUG] " + itr->second).c_str());
            }
            if (itr->first == LogLevel::LVL_INFO)
            {
                ImGui::TextColored({ 0, 1, 0, 1 }, ("[INFO] " + itr->second).c_str());
            }
            if (itr->first == LogLevel::LVL_WARN)
            {
                ImGui::TextColored({ 1, 0.9, 0, 1 }, ("[WARN] " + itr->second).c_str());
            }
            if (itr->first == LogLevel::LVL_ERROR)
            {
                ImGui::TextColored({ 1, 0, 0, 1 }, ("[ERROR] " + itr->second).c_str());
            }
        }

        ImGui::End();
    }

    void Console::Print(std::string message, LogLevel level)
    {
        switch (level)
        {
            case LightEngine::Console::LogLevel::LVL_INFO:
            {
                m_Messages.insert(std::pair <LogLevel, std::string>(LogLevel::LVL_INFO, message));
                break;
            }
            case LightEngine::Console::LogLevel::LVL_DEBUG:
            {
                m_Messages.insert(std::pair <LogLevel, std::string>(LogLevel::LVL_DEBUG, message));
                break;
            }
            case LightEngine::Console::LogLevel::LVL_WARN:
            {
                m_Messages.insert(std::pair <LogLevel, std::string>(LogLevel::LVL_WARN, message));
                break;
            }
            case LightEngine::Console::LogLevel::LVL_ERROR:
            {
                m_Messages.insert(std::pair <LogLevel, std::string>(LogLevel::LVL_ERROR, message));
                break;
            }
        }
    }

    void Console::ClearLog()
    {
        m_Messages.clear();
    }

}