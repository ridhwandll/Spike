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
#include <memory>
#include "LightEngine/Core/PlatformDetection.h"

#ifdef LE_DEBUG
    #if defined(LE_PLATFORM_WINDOWS)
        #define LE_DEBUGBREAK() __debugbreak()
    #elif defined(LE_PLATFORM_LINUX)
        #include<signal.h>
        #define LE_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "LightEngine doesn't support debugbreak on this platform!"
    #endif
    #define LE_ENABLE_ASSERTS
#else
    #define LE_DEBUGBREAK()
#endif

#ifdef LE_ENABLE_ASSERTS
    #define LE_ASSERT(x, ...) { if(!(x)) { LE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); LE_DEBUGBREAK(); } }
    #define LE_CORE_ASSERT(x, ...) { if(!(x)) { LE_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); LE_DEBUGBREAK(); } }
    #define LE_INTERNAL_ASSERT(x) { LE_CORE_LOG_CRITICAL(x); LE_DEBUGBREAK(); }
#else
    #define LE_ASSERT(x, ...)
    #define LE_CORE_ASSERT(x, ...)
    #define LE_INTERNAL_ASSERT(...)
#endif

#define BIT(x) (1 << x)

#define LE_BIND_EVENT_FN(fn) [this](auto&&... args)->decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace LightEngine
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
