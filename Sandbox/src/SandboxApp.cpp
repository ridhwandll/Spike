#include <LightEngine.h>

class SandBox : public LightEngine::Application
{
public:
	SandBox()
	{
	}
	~SandBox()
	{
	}

};

LightEngine::Application* LightEngine::CreateApplication()
{
	return new SandBox();
}