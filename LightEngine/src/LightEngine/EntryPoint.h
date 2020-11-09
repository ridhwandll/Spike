#pragma once

extern LightEngine::Application* LightEngine::CreateApplication();

int main(int argc, char** argv)
{
    LightEngine::Log::Init();
    auto app = LightEngine::CreateApplication();
    app->Run();
    delete app;
}
