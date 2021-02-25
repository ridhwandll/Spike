/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Physics2D
File Type      : h
File created on: 2021/01/22
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Scene/Entity.h"
#include "Spike/Core/Base.h"
#include "Spike/Scene/Scene.h"
#include "Spike/Core/Timestep.h"

namespace Spike
{
    enum class CollisionDetectionType
    {
        Discrete = 0, Continuous = 1
    };

    enum class SleepType
    {
        NeverSleep = 0, StartAwake = 1, StartAsleep = 2
    };

    class Physics2D
    {
    public:
        static void CreateScene(Scene* scene);
        static void Simulate(Timestep ts);
        static void Init();
        static void Shutdown();

        static void SetGravity(float gravity);
        static float GetGravity();
    private:
        static Scene* m_Scene;
        static Entity* m_Physics2DBodyEntityBuffer;
    };
}