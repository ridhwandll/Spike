//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/PlatformDetection.h"

#ifdef SPK_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN /* [Spike] Should we define this❓ [Spike] */
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
#endif

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Spike/Core/Base.h"
#include "Spike/Core/Log.h"

#ifdef SPK_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <winioctl.h>
#endif 
