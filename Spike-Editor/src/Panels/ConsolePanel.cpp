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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "ConsolePanel.h"
#include "UIUtils/UIUtils.h"

namespace Spike
{
    Console* Console::m_Console = new Console();

    Console::Console() {}

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

        if (ImGui::Button("Clear") || m_Messages.size() > 9999)
            ClearLog();

        ImGui::SameLine();
        GUI::DrawDynamicToggleButton(ICON_FK_TIMES, ICON_FK_CHECK, { 0.7f, 0.1f, 0.1f, 1.0f }, { 0.2f, 0.5f, 0.2f, 1.0f }, &m_ScrollLockEnabled);

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
            ImGui::TextUnformatted("Scroll lock");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        ImGui::SameLine();
        GUI::DrawColorChangingToggleButton(ICON_FK_PAPERCLIP, m_DisabledColor, m_EnabledColor, m_TraceColor, &m_TraceEnabled);
        ImGui::SameLine();
        GUI::DrawColorChangingToggleButton(ICON_FK_INFO_CIRCLE, m_DisabledColor, m_EnabledColor, m_InfoColor, &m_InfoEnabled);
        ImGui::SameLine();
        GUI::DrawColorChangingToggleButton(ICON_FK_BUG, m_DisabledColor, m_EnabledColor, m_DebugColor, &m_DebugEnabled);
        ImGui::SameLine();
        GUI::DrawColorChangingToggleButton(ICON_FK_EXCLAMATION_TRIANGLE, m_DisabledColor, m_EnabledColor, m_WarnColor, &m_WarningEnabled);
        ImGui::SameLine();
        GUI::DrawColorChangingToggleButton(ICON_FK_EXCLAMATION_CIRCLE, m_DisabledColor, m_EnabledColor, m_ErrorColor, &m_ErrorEnabled);

        ImGui::BeginChild(ICON_FK_LIST" Console", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto itr = m_Messages.begin(); itr != m_Messages.end(); ++itr)
        {
            switch (itr->first)
            {
            case Severity::Trace:
            {
                if (m_TraceEnabled)
                {
                    ImGui::TextColored(m_TraceColor, (ICON_FK_PAPERCLIP" " + itr->second).c_str());
                }
                break;
            }
            case Severity::Info:
                {
                    if (m_InfoEnabled)
                    {
                        ImGui::TextColored(m_InfoColor, (ICON_FK_INFO_CIRCLE" " + itr->second).c_str());
                    }
                    break;
                }
            case Severity::Debug:
                {
                    if (m_DebugEnabled)
                    {
                        ImGui::TextColored(m_DebugColor, (ICON_FK_BUG" " + itr->second).c_str());
                    }
                    break;
                }
            case Severity::Warning:
                {
                    if (m_WarningEnabled)
                    {
                        ImGui::TextColored(m_WarnColor, (ICON_FK_EXCLAMATION_TRIANGLE" " + itr->second).c_str());
                    }
                    break;
                }
            case Severity::Error:
                {
                    if (m_ErrorEnabled)
                    {
                        ImGui::TextColored(m_ErrorColor, (ICON_FK_EXCLAMATION_CIRCLE" " + itr->second).c_str());
                    }
                    break;
                }
            case Severity::Critical:
                {
                    // You can't toggle off the critical errors!
                    ImGui::TextColored(m_CriticalColor, (ICON_FK_EXCLAMATION_CIRCLE" " + itr->second).c_str());
                    break;
                }
            }
        }

        if (m_ScrollLockEnabled)
            ImGui::SetScrollY(ImGui::GetScrollMaxY() + 100);

        ImGui::EndChild();
        ImGui::End();

    }

    void Console::Print(const String& message, Severity level)
    {
        m_Messages.emplace_back(std::pair<Severity, String>(level, message));
    }

    void Console::ClearLog()
    {
        m_Messages.clear();
    }

}