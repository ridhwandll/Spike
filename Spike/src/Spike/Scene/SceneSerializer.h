//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Scene.h"

namespace Spike
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const String& filepath);
        bool Deserialize(const String& filepath);
    private:
        Ref<Scene> m_Scene;
    };
}