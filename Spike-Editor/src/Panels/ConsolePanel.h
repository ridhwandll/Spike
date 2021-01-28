/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ConsolePanel
File Type      : h
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
#pragma once
#include <vector>
#include <string>
#include <FontAwesome.h>
#include <imgui.h>

namespace Spike
{
    class Console
    {
    public:
        ~Console();
        enum class LogLevel
        {
            LVL_INFO, LVL_DEBUG, LVL_WARN, LVL_ERROR, LVL_CRITICAL
        };

        static Console* Get();

        void OnImGuiRender();
        void Print(const std::string& message, LogLevel level = LogLevel::LVL_DEBUG);

    private:
        Console();
        void ClearLog();
    private:
        static Console* m_Console;
        std::vector<std::pair<LogLevel, std::string>> m_Messages{};
        bool m_ScrollLockEnabled = true;

        //Colors
        ImVec4 m_InfoColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        ImVec4 m_DebugColor = { 0.0f, 0.5f, 1.0f, 1.0f };
        ImVec4 m_WarnColor = { 1.0f, 0.9f, 0.0f, 1.0f };
        ImVec4 m_ErrorColor = { 1.0f, 0.2f, 0.1f, 1.0f };
        ImVec4 m_CriticalColor = { 0.5f, 0.0f, 0.7f, 1.0f };
        ImVec4 m_EnabledColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        ImVec4 m_DisabledColor = { 0.5f, 0.2f, 0.2f, 1.0f };

        //Bool switches
        bool m_WarningEnabled = true;
        bool m_InfoEnabled = true;
        bool m_DebugEnabled = true;
        bool m_ErrorEnabled = true;
    };
}