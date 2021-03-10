//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Spike/Renderer/RendererAPISwitch.h"

#ifdef RENDERER_API_OPENGL
    #define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_opengl3.cpp"
#elif defined RENDERER_API_DX11
    #include "backends/imgui_impl_dx11.cpp"
#endif

#include "backends/imgui_impl_glfw.cpp" 