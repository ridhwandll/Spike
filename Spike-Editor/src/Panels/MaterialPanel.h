//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Scene/Entity.h"

namespace Spike
{
    class MaterialPanel
    {
    public:
        MaterialPanel() = default;
        ~MaterialPanel() = default;

        void OnImGuiRender(Entity& selectedEntity);
    };
}