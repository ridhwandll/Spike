//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Base.h"
#include "Spike/Core/Log.h"
#include <imgui.h>
#include <FontAwesome.h>
#include <vector>
#include <string>

namespace Spike
{
    class Console
    {
    public:
        ~Console();
        static Console* Get();
        void OnImGuiRender(bool* show);
        void Print(const String& message, Severity level = Severity::Info);

    private:
        Console();
        void ClearLog();
    private:
        static Console* m_Console;
        std::vector<std::pair<Severity, String>> m_Messages{};
        bool m_ScrollLockEnabled = true;

        //Colors
        ImVec4 m_TraceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        ImVec4 m_InfoColor = { 0.1f, 0.9f, 0.1f, 1.0f };
        ImVec4 m_DebugColor = { 0.0f, 0.5f, 1.0f, 1.0f };
        ImVec4 m_WarnColor = { 1.0f, 0.9f, 0.0f, 1.0f };
        ImVec4 m_ErrorColor = { 1.0f, 0.2f, 0.1f, 1.0f };
        ImVec4 m_CriticalColor = { 0.5f, 0.0f, 0.7f, 1.0f };
        ImVec4 m_EnabledColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        ImVec4 m_DisabledColor = { 0.5f, 0.2f, 0.2f, 1.0f };

        //Bool switches
        bool m_WarningEnabled = true;
        bool m_TraceEnabled = true;
        bool m_InfoEnabled = true;
        bool m_DebugEnabled = true;
        bool m_ErrorEnabled = true;
    };
}