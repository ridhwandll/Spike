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
#include "ConsolePanel.h"
#include "../UIUtils/UIUtils.h"

namespace Spike
{
    Console* Console::m_Console = new Console();

    Console::Console()
    {
    }

    Console::~Console()
    {
        delete m_Console;
    }

    Console* Console::Get()
    {
        return m_Console;
    }

    void Console::OnImGuiRender()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::Begin(ICON_FK_LIST" Console");

        if (ImGui::Button(ICON_FK_COGS))
            ImGui::OpenPopup("Console Settings");

        if(ImGui::BeginPopup("Console Settings"))
        {
            if (ImGui::Button("Clear"))
                ClearLog();

            ImGui::Checkbox("Scroll Lock", &m_ScrollLockEnabled);
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        //TODO: Button Shows effects if they are toggled on
        DrawToggleButton(ICON_FK_INFO_CIRCLE, m_InfoColor, [this]() { m_InfoEnabled ^= true; });               //Info
        ImGui::SameLine();
        DrawToggleButton(ICON_FK_BUG, m_DebugColor, [this]() { m_DebugEnabled ^= true; });                     //Bug
        ImGui::SameLine();
        DrawToggleButton(ICON_FK_EXCLAMATION_TRIANGLE, m_WarnColor, [this]() { m_WarningEnabled ^= true; });   //Warn
        ImGui::SameLine();
        DrawToggleButton(ICON_FK_EXCLAMATION_CIRCLE, m_ErrorColor, [this]() { m_ErrorEnabled ^= true; });      //Error

        ImGui::BeginChild(ICON_FK_LIST" Console");

        std::string type;
        for (auto itr = m_Messages.begin(); itr != m_Messages.end(); ++itr)
        {
            switch (itr->first)
            {
                case LogLevel::LVL_INFO:
                {
                    if (m_InfoEnabled)
                    {
                        type = ICON_FK_INFO_CIRCLE" [INFO] ";
                        ImGui::TextColored(m_InfoColor, (type + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_DEBUG:
                {
                    if (m_DebugEnabled)
                    {
                        type = ICON_FK_BUG" [DEBUG] ";
                        ImGui::TextColored(m_DebugColor, (type + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_WARN:
                {
                    if (m_WarningEnabled)
                    {
                        type = ICON_FK_EXCLAMATION_TRIANGLE" [WARNING] ";
                        ImGui::TextColored(m_WarnColor, (type + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_ERROR:
                {
                    if (m_ErrorEnabled)
                    {
                        type = ICON_FK_EXCLAMATION_CIRCLE" [ERROR] ";
                        ImGui::TextColored(m_ErrorColor, (type + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_CRITICAL:
                {
                    // You can't toggle off the critical errors!
                    type = ICON_FK_EXCLAMATION_CIRCLE" [CRITICAL] ";
                    ImGui::TextColored(m_CriticalColor, (type + itr->second).c_str());
                    break;
                }
            }
        }

        if (m_ScrollLockEnabled)
            ImGui::SetScrollY(ImGui::GetScrollMaxY() + 100);

        ImGui::EndChild();
        ImGui::End();

    }

    void Console::Print(std::string message, LogLevel level)
    {
        switch (level)
        {
            case LogLevel::LVL_INFO:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_INFO, message));
                break;
            }
            case LogLevel::LVL_DEBUG:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_DEBUG, message));
                break;
            }
            case LogLevel::LVL_WARN:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_WARN, message));
                break;
            }
            case LogLevel::LVL_ERROR:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_ERROR, message));
                break;
            }
            case LogLevel::LVL_CRITICAL:
            {
                m_Messages.emplace_back(std::pair <LogLevel, std::string>(LogLevel::LVL_CRITICAL, message));
                break;
            }
        }
    }

    void Console::ClearLog()
    {
        m_Messages.clear();
    }

}