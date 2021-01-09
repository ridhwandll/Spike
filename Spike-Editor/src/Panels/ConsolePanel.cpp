/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ConsolePanel
File Type      : cpp
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
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

        ImGui::BeginChild(ICON_FK_LIST" Console", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto itr = m_Messages.begin(); itr != m_Messages.end(); ++itr)
        {
            switch (itr->first)
            {
                case LogLevel::LVL_INFO:
                {
                    if (m_InfoEnabled)
                    {
                        ImGui::TextColored(m_InfoColor, (ICON_FK_INFO_CIRCLE" [INFO] " + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_DEBUG:
                {
                    if (m_DebugEnabled)
                    {
                        ImGui::TextColored(m_DebugColor, (ICON_FK_BUG" [DEBUG] " + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_WARN:
                {
                    if (m_WarningEnabled)
                    {
                        ImGui::TextColored(m_WarnColor, (ICON_FK_EXCLAMATION_TRIANGLE" [WARNING] " + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_ERROR:
                {
                    if (m_ErrorEnabled)
                    {
                        ImGui::TextColored(m_ErrorColor, (ICON_FK_EXCLAMATION_CIRCLE" [ERROR] " + itr->second).c_str());
                    }
                    break;
                }
                case LogLevel::LVL_CRITICAL:
                {
                    // You can't toggle off the critical errors!
                    ImGui::TextColored(m_CriticalColor, (ICON_FK_EXCLAMATION_CIRCLE" [CRITICAL] " + itr->second).c_str());
                    break;
                }
            }
        }

        if (m_ScrollLockEnabled)
            ImGui::SetScrollY(ImGui::GetScrollMaxY() + 100);

        ImGui::EndChild();
        ImGui::End();

    }

    void Console::Print(const std::string& message, LogLevel level)
    {
        m_Messages.emplace_back(std::pair <LogLevel, std::string>(level, message));
    }

    void Console::ClearLog()
    {
        m_Messages.clear();
    }

}