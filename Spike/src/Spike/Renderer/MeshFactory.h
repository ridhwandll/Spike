//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Mesh.h"

namespace Spike
{
    class MeshFactory
    {
    public:
        static Ref<Mesh> CreateCube(const glm::vec3& size);
    };
}
