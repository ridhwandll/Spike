//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include <vector>

namespace Spike
{
    class Entity;
    class ProfilerPanel
    {
    public:
        void OnImGuiRender(bool* show);

    private:
        float m_Time = 0.0f;
        bool m_VSync;
        float m_FPSValues[50];
        std::vector<float> m_FrameTimes;
    };
}
