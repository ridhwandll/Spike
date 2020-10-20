#pragma once


extern LightEngine::Application* LightEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = LightEngine::CreateApplication();
	app->Run();
	delete app;
}
