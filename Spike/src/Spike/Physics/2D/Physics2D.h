//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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

    class ContactListener2D;
    class Physics2D
    {
    public:
        static void CreateScene(Scene* scene);
        static void Simulate(Timestep ts);
        static void Init();
        static void Shutdown();

        static ContactListener2D GetContactListener();
        static void SetGravity(float gravity);
        static float GetGravity();
    private:
        static ContactListener2D m_ContactListener;
        static Scene* m_Scene;
        static Entity* m_Physics2DBodyEntityBuffer;
    };
}