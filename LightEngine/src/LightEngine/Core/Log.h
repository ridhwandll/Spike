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
#pragma once
#include "Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace LightEngine
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
#define LE_CORE_LOG_TRACE(...)    ::LightEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LE_CORE_LOG_DEBUG(...)    ::LightEngine::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LE_CORE_LOG_INFO(...)     ::LightEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LE_CORE_LOG_WARN(...)     ::LightEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LE_CORE_LOG_ERROR(...)    ::LightEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LE_CORE_LOG_CRITICAL(...) ::LightEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client Log Macros
#define LE_LOG_TRACE(...)         ::LightEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LE_LOG_DEBUG(...)         ::LightEngine::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LE_LOG_INFO(...)          ::LightEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define LE_LOG_WARN(...)          ::LightEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LE_LOG_ERROR(...)         ::LightEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define LE_LOG_CRITICAL(...)      ::LightEngine::Log::GetClientLogger()->critical(__VA_ARGS__)