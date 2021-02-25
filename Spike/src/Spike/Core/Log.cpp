/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Log
File Type      : cpp
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Spike/Core/Log.h"
#include "Panels/ConsolePanel.h"
#include <cstring>
#include <ctime>
#include <filesystem>
#include <stdarg.h>
#include <stdio.h>

namespace Spike
{
    Logger Logger::s_CoreLogger = Logger("Spike");
    std::vector<std::string>     Logger::s_Buffer;

    bool Logger::s_LogToFile = false;
    bool Logger::s_LogToConsole = true;
    bool Logger::s_LogToEditorConsole = true;

    const char* Logger::s_PreviousFile = "Logs/SpikeEngine-CurrentLogs.spikeLog";
    const char* Logger::s_CurrentFile = "Logs/SpikeEngine-Logs.spikeLog";

    Logger::Logger(const char* name)
        : m_Name(name) {}

    void Logger::Log(Severity severity, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Logger::Log(this->m_Name, severity, format, args);
        va_end(args);
    }

    void Logger::LogTrace(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Logger::Log(this->m_Name, Severity::Trace, format, args);
        va_end(args);
    }

    void Logger::LogInfo(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Logger::Log(this->m_Name, Severity::Info, format, args);
        va_end(args);
    }
    void Logger::LogDebug(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Logger::Log(this->m_Name, Severity::Debug, format, args);
        va_end(args);
    }

    void Logger::LogWarning(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Logger::Log(this->m_Name, Severity::Warning, format, args);
        va_end(args);
    }

    void Logger::LogError(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Logger::Log(this->m_Name, Severity::Error, format, args);
        va_end(args);
    }

    void Logger::LogCritical(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Logger::Log(this->m_Name, Severity::Critical, format, args);
        va_end(args);
    }

    void Logger::Init()
    {
        if (std::filesystem::exists(Logger::s_CurrentFile))
        {
            if (std::filesystem::exists(Logger::s_PreviousFile))
                std::filesystem::remove(Logger::s_PreviousFile);

            if (rename(Logger::s_CurrentFile, Logger::s_PreviousFile))
                Logger("Logger").Log(Severity::Debug, "Failed to rename log file %s to %s", Logger::s_CurrentFile, Logger::s_PreviousFile);
        }
    }

    void Logger::Shutdown()
    {
        Flush();
    }

    void Logger::Flush()
    {
        if (!Logger::s_LogToFile)
            return;

        std::filesystem::path filepath{ Logger::s_CurrentFile };
        std::filesystem::create_directories(filepath.parent_path());

        FILE* file = fopen(Logger::s_CurrentFile, "a");
        if (file)
        {
            for (auto message : Logger::s_Buffer)
                fwrite(message.c_str(), sizeof(char), message.length(), file);
            fclose(file);
            Logger::s_Buffer.clear();
        }
        else
        {
            Logger::s_LogToFile = false;
        }
    }

    uint32_t Logger::GetSeverityMaxBufferCount(Severity severity)
    {
        switch (severity)
        {
            case Severity::Trace:
                return 100;
            case Severity::Info:
                return 100;
            case Severity::Debug:
                return 50;
            case Severity::Warning:
                return 10;
            case Severity::Error:
                return 0;
            case Severity::Critical:
                return 0;
        }
        return 0;
    }

    const char* Logger::GetSeverityID(Severity severity)
    {
        switch (severity)
        {
            case Severity::Trace:
                return "TRACE";
            case Severity::Info:
                return "INFO";
            case Severity::Debug:
                return "DEBUG";
            case Severity::Warning:
                return "WARNING";
            case Severity::Error:
                return "ERROR";
            case  Severity::Critical:
                return "CRITICAL";
        }
        return "Unknown Severity";
    }

    const char* Logger::GetSeverityConsoleColor(Severity severity)
    {
       /*
        * Console Colors https://stackoverflow.com/questions/4053837
        * Name            FG  BG
        * Black           30  40
        * Red             31  41
        * Green           32  42
        * Yellow          33  43
        * Blue            34  44
        * Magenta         35  45
        * Cyan            36  46
        * White           37  47
        * Bright Black    90  100
        * Bright Red      91  101
        * Bright Green    92  102
        * Bright Yellow   93  103
        * Bright Blue     94  104
        * Bright Magenta  95  105
        * Bright Cyan     96  106
        * Bright White    97  107
        */
        switch (severity)
        {
            case Severity::Trace:
                return "\033[0;97m";
            case Severity::Info:
                return "\033[0;92m";
            case Severity::Debug:
                return "\033[0;36m";
            case Severity::Warning:
                return "\033[0;93m";
            case Severity::Error:
                return "\033[0;91m";
            case Severity::Critical:
                return "\033[0;101m";
        }
        return "\033[0;97m";
    }

    void Logger::Log(const char* name, Severity severity, const char* format, va_list args)
    {
        uint32_t length = vsnprintf(nullptr, 0, format, args) + 1;
        char* buf = new char[length];
        vsnprintf(buf, length, format, args);

        std::string message(buf);
        delete[] buf;

        std::vector<std::string> messages;

        uint32_t lastIndex = 0;
        for (uint32_t i = 0; i < message.length(); i++)
        {
            if (message[i] == '\n')
            {
                messages.push_back(message.substr(lastIndex, i - lastIndex));
                lastIndex = i + 1;
            }
            else if (i == message.length() - 1)
            {
                messages.push_back(message.substr(lastIndex));
            }
        }

        for (std::string msg : messages)
        {
            std::string logMsg = "";
            std::string systemConsoleMsg = "";
            std::string editorConsoleMsg = "";

            constexpr uint32_t timeBufferSize = 16;
            std::time_t        currentTime = std::time(nullptr);
            char               timeBuffer[timeBufferSize];

            if (Logger::s_LogToFile)
                logMsg += "[" + std::string(name) + "]";
            if (Logger::s_LogToConsole)
                systemConsoleMsg += std::string(Logger::GetSeverityConsoleColor(severity)) + "[" + std::string(name) + "]";
            if (Logger::s_LogToEditorConsole)
                editorConsoleMsg += "[" + std::string(name) + "]";

            if (std::strftime(timeBuffer, timeBufferSize, "[%H:%M:%S]", std::localtime(&currentTime)))
            {
                if (Logger::s_LogToFile)
                    logMsg += timeBuffer;
                if (Logger::s_LogToConsole)
                    systemConsoleMsg += timeBuffer;
                if (Logger::s_LogToEditorConsole)
                    editorConsoleMsg += timeBuffer;
            }

            if (Logger::s_LogToFile)
                logMsg += " " + std::string(Logger::GetSeverityID(severity)) + ": " + msg + "\n";
            if (Logger::s_LogToConsole)
                systemConsoleMsg += " " + std::string(Logger::GetSeverityID(severity)) + ": " + msg + +"\033[0m " + "\n";
            if (Logger::s_LogToEditorConsole)
                editorConsoleMsg += " " + std::string(Logger::GetSeverityID(severity)) + ": " + msg;

            if (Logger::s_LogToFile)
                Logger::s_Buffer.push_back(logMsg);
            if (Logger::s_LogToConsole)
                printf("%s", systemConsoleMsg.c_str());
            if (Logger::s_LogToEditorConsole)
                Console::Get()->Print(editorConsoleMsg, severity);
        }

        if (Logger::s_LogToFile)
            if (Logger::s_Buffer.size() > Logger::GetSeverityMaxBufferCount(severity))
                Flush();
    }
}