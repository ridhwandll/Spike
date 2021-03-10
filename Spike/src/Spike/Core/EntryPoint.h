//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once

#ifdef SPK_PLATFORM_WINDOWS

extern Spike::Application* Spike::CreateApplication();

int main(int argc, char** argv)
{
    Spike::Logger::Init();
    auto app = Spike::CreateApplication();
    app->Run();
    Spike::Logger::Shutdown();
    delete app;
}

#endif // SPK_PLATFORM_WINDOWS
