//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include <Spike.h>
#include <Spike/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Spike {

    class SpikeEditor : public Application
    {
    public:
        SpikeEditor()
            :Application("Spike Editor")
        {
            Application::SetAppIcon("Resources/Branding/SpikeLogo.png");
            PushLayer(new EditorLayer());
        }

        ~SpikeEditor()
        {
        }
    };

    Application* CreateApplication()
    {
        return new SpikeEditor();
    }

}