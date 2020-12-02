#pragma once
#include "LightEngine/Core/PlatformDetection.h"

#ifdef LE_PLATFORM_WINDOWS
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

#include "LightEngine/Core/Base.h"
#include "LightEngine/Core/Log.h"
#include "LightEngine/Debug/Instrumentor.h"

#ifdef LE_PLATFORM_WINDOWS
    #include <Windows.h>
#endif 
