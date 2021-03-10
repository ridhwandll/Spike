//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Base.h"
#include "Spike/Core/Ref.h"

namespace Spike
{
    enum class Severity
    {
        Trace,
        Info,
        Debug,
        Warning,
        Error,
        Critical
    };

    class Logger
    {
    public:
        Logger(const char* name);

        void Log(Severity severity, const char* format, ...);
        void LogTrace(const char* format, ...);
        void LogInfo(const char* format, ...);
        void LogDebug(const char* format, ...);
        void LogWarning(const char* format, ...);
        void LogError(const char* format, ...);
        void LogCritical(const char* format, ...);
        inline static Logger GetCoreLogger() { return s_CoreLogger; };

        void SetLogToFile(bool value) { s_LogToFile = value; }
        void SetLogToSystemConsole(bool value) { s_LogToConsole = value; }
        void SetLogToEditorConsole(bool value) { s_LogToEditorConsole = value; }
    public:
        static void Init();
        static void Shutdown();
        static void Flush();

    private:
        static uint32_t GetSeverityMaxBufferCount(Severity severity);
        static const char* GetSeverityID(Severity severity);
        static const char* GetSeverityConsoleColor(Severity severity);
        static void Log(const char* name, Severity severity, const char* format, va_list args);

    private:
        const char* m_Name;
    private:
        static Logger s_CoreLogger;
        static std::vector<std::string> s_Buffer;

        static bool        s_LogToFile;
        static bool        s_LogToConsole;
        static bool        s_LogToEditorConsole;
        static const char* s_PreviousFile;
        static const char* s_CurrentFile;
    };
}

/* [Spike] Formatters
*   %c                  Character
    %d                  Signed integer
    %e or %E            Scientific notation of floats
    %f                  Float values
    %g or %G            Similar as %e or %E
    %hi                 Signed integer (short)
    %hu                 Unsigned Integer (short)
    %i                  Unsigned integer
    %l or %ld or %li    Long
    %lf                 Double
    %Lf                 Long double
    %lu                 Unsigned int or unsigned long
    %lli or %lld        Long long
    %llu                Unsigned long long
    %o                  Octal representation
    %p                  Pointer
    %s                  String
    %u                  Unsigned int
    %x or %X            Hexadecimal representation
    %n                  Prints nothing
    %%                  Prints % character
 * [Spike] */

#define SPK_CORE_LOG_TRACE(...)    ::Spike::Logger::GetCoreLogger().LogTrace(__VA_ARGS__)
#define SPK_CORE_LOG_DEBUG(...)    ::Spike::Logger::GetCoreLogger().LogDebug(__VA_ARGS__)
#define SPK_CORE_LOG_INFO(...)     ::Spike::Logger::GetCoreLogger().LogInfo(__VA_ARGS__)
#define SPK_CORE_LOG_WARN(...)     ::Spike::Logger::GetCoreLogger().LogWarning(__VA_ARGS__)
#define SPK_CORE_LOG_ERROR(...)    ::Spike::Logger::GetCoreLogger().LogError(__VA_ARGS__)
#define SPK_CORE_LOG_CRITICAL(...) ::Spike::Logger::GetCoreLogger().LogCritical(__VA_ARGS__)