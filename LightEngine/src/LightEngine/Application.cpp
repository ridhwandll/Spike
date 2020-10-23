#include "Application.h"

#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/Log.h"

namespace LightEngine
{
    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        if (e.IsInCategory(EventCategoryApplication))
        {
            LE_TRACE(e);
        }
        if (e.IsInCategory(EventCategoryInput))
        {
            LE_TRACE(e);
        }
        while (true);
    }
}
