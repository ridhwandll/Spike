#pragma once


extern LightEngine::Application* LightEngine::CreateApplication();

int main(int argc, char** argv)
{
    LightEngine::Log::Init();
    LE_CORE_INFO("Initialized Log!");
    LE_DEBUG("Running");
    auto app = LightEngine::CreateApplication();
    app->Run();
    delete app;
}
