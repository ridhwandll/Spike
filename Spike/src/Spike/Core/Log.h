/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Log
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Spike
{
    class  Log
    {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr < spdlog::logger> s_CoreLogger;
        static std::shared_ptr < spdlog::logger> s_ClientLogger;
    };
}

//Core Log Macros
#define SPK_CORE_LOG_TRACE(...)    ::Spike::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SPK_CORE_LOG_DEBUG(...)    ::Spike::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define SPK_CORE_LOG_INFO(...)     ::Spike::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SPK_CORE_LOG_WARN(...)     ::Spike::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SPK_CORE_LOG_ERROR(...)    ::Spike::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SPK_CORE_LOG_CRITICAL(...) ::Spike::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client Log Macros
#define SPK_LOG_TRACE(...)         ::Spike::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SPK_LOG_DEBUG(...)         ::Spike::Log::GetClientLogger()->debug(__VA_ARGS__)
#define SPK_LOG_INFO(...)          ::Spike::Log::GetClientLogger()->info(__VA_ARGS__)
#define SPK_LOG_WARN(...)          ::Spike::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SPK_LOG_ERROR(...)         ::Spike::Log::GetClientLogger()->error(__VA_ARGS__)
#define SPK_LOG_CRITICAL(...)      ::Spike::Log::GetClientLogger()->critical(__VA_ARGS__)