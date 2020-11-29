#include <LightEngine.h>
#include <LightEngine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace LightEngine {

    class LightEngineEditor : public Application
    {
    public:
        LightEngineEditor()
            :Application("LightEngine Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~LightEngineEditor()
        {
        }
    };

    Application* CreateApplication()
    {
        return new LightEngineEditor();
    }

}