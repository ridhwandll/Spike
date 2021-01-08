/*****************************************************************************/
/*                            Spike SourceCode                               */
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
#include <memory>
#include "Spike/Core/PlatformDetection.h"

#ifdef SPK_DEBUG
    #if defined(SPK_PLATFORM_WINDOWS)
        #define SPK_DEBUGBREAK() __debugbreak()
    #elif defined(SPK_PLATFORM_LINUX)
        #include<signal.h>
        #define SPK_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Spike doesn't support debugbreak on this platform!"
    #endif
    #define SPK_ENABSPK_ASSERTS
#else
    #define SPK_DEBUGBREAK()
#endif

#ifdef SPK_ENABSPK_ASSERTS
    #define SPK_ASSERT(x, ...) { if(!(x)) { SPK_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); SPK_DEBUGBREAK(); } }
    #define SPK_CORE_ASSERT(x, ...) { if(!(x)) { SPK_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); SPK_DEBUGBREAK(); } }
    #define SPK_INTERNAL_ASSERT(x) { SPK_CORE_LOG_CRITICAL(x); SPK_DEBUGBREAK(); }
#else
    #define SPK_ASSERT(x, ...)
    #define SPK_CORE_ASSERT(x, ...)
    #define SPK_INTERNAL_ASSERT(...)
#endif

#define BIT(x) (1 << x)

#define SPK_BIND_EVENT_FN(fn) [this](auto&&... args)->decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Spike
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}
