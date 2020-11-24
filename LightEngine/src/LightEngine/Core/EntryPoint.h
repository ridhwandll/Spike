#pragma once

#ifdef LE_PLATFORM_WINDOWS

extern LightEngine::Application* LightEngine::CreateApplication();

int main(int argc, char** argv)
{
    LightEngine::Log::Init();
    LE_PROFILE_BEGIN_SESSION("Startup", "LightEngineProfile-Startup.json");
    auto app = LightEngine::CreateApplication();
    LE_PROFILE_END_SESSION();

    LE_PROFILE_BEGIN_SESSION("Runtime", "LightEngineProfile-Runtime.json");
    app->Run();
    LE_PROFILE_END_SESSION();

    LE_PROFILE_BEGIN_SESSION("Shutdown", "LightEngineProfile-Shutdown.json");
    delete app;
    LE_PROFILE_END_SESSION();
}

#endif // LE_PLATFORM_WINDOWS
