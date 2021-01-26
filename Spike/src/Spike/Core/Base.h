/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Base
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
    #define SPK_INTERNAL_ASSERT(x) { if(!(x)) { SPK_DEBUGBREAK(); SPK_CORE_LOG_CRITICAL("Assertion Failed! RIP"); } }
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
    typedef uint8_t byte;
}
