/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Physics2D
File Type      : cpp
File created on: 2021/01/22
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Physics2D.h"
#include <box2D/box2D.h>

namespace Spike
{
    Entity* Physics2D::m_Physics2DBodyEntityBuffer = nullptr;
    Scene* Physics2D::m_Scene = nullptr;

    class ContactListener2D : public b2ContactListener
    {
    public:
        /* [Spike] Called when contact begins [Spike] */
        void BeginContact(b2Contact* contact) override
        {
            /* [Spike] Collision between 2 entities, so entityA and entityB [Spike] */
            Entity& entityA = *(Entity*)contact->GetFixtureA()->GetBody()->GetUserData();
            Entity& entityB = *(Entity*)contact->GetFixtureB()->GetBody()->GetUserData();

            if (entityA.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(entityA.GetComponent<ScriptComponent>().ModuleName))
                ScriptEngine::OnCollision2DBegin(entityA);

            if (entityB.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(entityB.GetComponent<ScriptComponent>().ModuleName))
                ScriptEngine::OnCollision2DBegin(entityB);
        }

        /* [Spike] Called when contact ends [Spike] */
        void EndContact(b2Contact* contact) override
        {
            /* [Spike] Collision between 2 entities, so entityA and entityB [Spike] */
            Entity& entityA = *(Entity*)contact->GetFixtureA()->GetBody()->GetUserData();
            Entity& entityB = *(Entity*)contact->GetFixtureB()->GetBody()->GetUserData();

            if (entityA.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(entityA.GetComponent<ScriptComponent>().ModuleName))
                ScriptEngine::OnCollision2DEnd(entityA);

            if (entityB.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(entityB.GetComponent<ScriptComponent>().ModuleName))
                ScriptEngine::OnCollision2DEnd(entityB);
        }

        virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
        {
            B2_NOT_USED(contact);
            B2_NOT_USED(oldManifold);
        }

        virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
        {
            B2_NOT_USED(contact);
            B2_NOT_USED(impulse);
        }
    };

    ContactListener2D Physics2D::m_ContactListener;

    struct Box2DWorldComponent
    {
        Box2DWorldComponent() = default;
        Scope<b2World> World;
    };

    void Physics2D::CreateScene(Scene* scene)
    {
        m_Scene = scene;
        Box2DWorldComponent& box2DWorld =
            m_Scene->m_Registry.emplace<Box2DWorldComponent>(m_Scene->m_SceneEntity, CreateScope<b2World>(b2Vec2{ 0.0f, -9.8f }));
        box2DWorld.World->SetContactListener(&m_ContactListener);
    }

    void Physics2D::Simulate()
    {
        auto scene = m_Scene->m_Registry.view<Box2DWorldComponent>();
        auto& box2DWorld = m_Scene->m_Registry.get<Box2DWorldComponent>(scene.front()).World;
        int32_t velocityIterations = 8;
        int32_t positionIterations = 2;

        // Update all the Rigidbody2D's
        {
            auto view = m_Scene->m_Registry.view<TransformComponent, RigidBody2DComponent>();
            for (auto entity : view)
            {
                Entity e = { entity, m_Scene };
                auto& rb2d = e.GetComponent<RigidBody2DComponent>();
                b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);

                auto& position = body->GetPosition();
                auto& transform = e.GetComponent<TransformComponent>();

                body->SetTransform({ transform.Translation.x, transform.Translation.y }, transform.Rotation.z);
            }

            box2DWorld->Step(0.02f, velocityIterations, positionIterations);

            for (auto entity : view)
            {
                auto [transform, rb2d] = view.get<TransformComponent, RigidBody2DComponent>(entity);

                b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
                auto& position = body->GetPosition();

                transform.Translation.x = position.x;
                transform.Translation.y = position.y;
                transform.Rotation.z = body->GetAngle();
                body->SetGravityScale(rb2d.Gravity);
            }
        }
    }

    void Physics2D::Init()
    {
        auto sceneView = m_Scene->m_Registry.view<Box2DWorldComponent>();
        auto& world = m_Scene->m_Registry.get<Box2DWorldComponent>(sceneView.front()).World;

        {
            auto view = m_Scene->m_Registry.view<RigidBody2DComponent>();
            m_Physics2DBodyEntityBuffer = new Entity[view.size()];
            uint32_t physicsBodyEntityBufferIndex = 0;
            for (auto entity : view)
            {
                Entity e = { entity, m_Scene };
                UUID entityID = e.GetComponent<IDComponent>().ID;
                TransformComponent& transform = e.GetComponent<TransformComponent>();
                auto& rigidBody2D = m_Scene->m_Registry.get<RigidBody2DComponent>(entity);

                b2BodyDef bodyDef;
                if (rigidBody2D.BodyType == RigidBody2DComponent::Type::Static)
                    bodyDef.type = b2_staticBody;
                else if (rigidBody2D.BodyType == RigidBody2DComponent::Type::Dynamic)
                    bodyDef.type = b2_dynamicBody;
                else if (rigidBody2D.BodyType == RigidBody2DComponent::Type::Kinematic)
                    bodyDef.type = b2_kinematicBody;
                bodyDef.position.Set(transform.Translation.x, transform.Translation.y);

                bodyDef.angle = transform.Rotation.z;

                b2Body* body = world->CreateBody(&bodyDef);
                body->SetFixedRotation(rigidBody2D.FixedRotation);
                Entity* entityStorage = &m_Physics2DBodyEntityBuffer[physicsBodyEntityBufferIndex++];
                *entityStorage = e;
                body->SetUserData((void*)entityStorage);
                rigidBody2D.RuntimeBody = body;

                //CollisionDetectionType switches
                switch (rigidBody2D.CollisionDetection)
                {
                    case CollisionDetectionType::Discrete:
                    {
                        body->SetBullet(false);
                        break;
                    }
                    case CollisionDetectionType::Continuous:
                    {
                        body->SetBullet(true);
                        break;
                    }
                }

                //SleepType switches
                switch (rigidBody2D.Sleeptype)
                {
                    case SleepType::NeverSleep:
                    {
                        body->SetSleepingAllowed(false);
                        body->SetAwake(true);
                        break;
                    }
                    case SleepType::StartAsleep:
                    {
                        body->SetSleepingAllowed(true);
                        body->SetAwake(false);
                        break;
                    }
                    case SleepType::StartAwake:
                    {
                        body->SetSleepingAllowed(true);
                        body->SetAwake(true);
                        break;
                    }
                }
            }
        }

        {
            auto view = m_Scene->m_Registry.view<BoxCollider2DComponent>();
            for (auto entity : view)
            {
                Entity e = { entity, m_Scene };
                auto& transform = e.Transform();

                auto& boxCollider2D = m_Scene->m_Registry.get<BoxCollider2DComponent>(entity);
                if (e.HasComponent<RigidBody2DComponent>())
                {
                    auto& rigidBody2D = e.GetComponent<RigidBody2DComponent>();
                    SPK_CORE_ASSERT(rigidBody2D.RuntimeBody, "Body doesn't exist!");
                    b2Body* body = static_cast<b2Body*>(rigidBody2D.RuntimeBody);

                    b2PolygonShape polygonShape;
                    polygonShape.SetAsBox(((boxCollider2D.Size.x * boxCollider2D.Scale.x) / 2), ((boxCollider2D.Size.y * boxCollider2D.Scale.y) / 2), (b2Vec2&)boxCollider2D.Offset, 0.0f);

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &polygonShape;
                    fixtureDef.density = boxCollider2D.Density;
                    fixtureDef.friction = boxCollider2D.Friction;
                    body->CreateFixture(&fixtureDef);
                }
            }
        }

        {
            auto view = m_Scene->m_Registry.view<CircleCollider2DComponent>();
            for (auto entity : view)
            {
                Entity e = { entity, m_Scene };
                auto& transform = e.Transform();

                auto& circleCollider2D = m_Scene->m_Registry.get<CircleCollider2DComponent>(entity);
                if (e.HasComponent<RigidBody2DComponent>())
                {
                    auto& rigidBody2D = e.GetComponent<RigidBody2DComponent>();
                    SPK_CORE_ASSERT(rigidBody2D.RuntimeBody, "Body doesn't exist!");
                    b2Body* body = static_cast<b2Body*>(rigidBody2D.RuntimeBody);

                    b2CircleShape circleShape;
                    circleShape.m_radius = circleCollider2D.Radius;

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &circleShape;
                    fixtureDef.density = circleCollider2D.Density;
                    fixtureDef.friction = circleCollider2D.Friction;
                    body->CreateFixture(&fixtureDef);
                }
            }
        }

    }

    void Physics2D::Shutdown()
    {
        delete[] m_Physics2DBodyEntityBuffer;
    }

    ContactListener2D Physics2D::GetContactListener()
    {
        return m_ContactListener;
    }

    void Physics2D::SetGravity(float gravity)
    {
        m_Scene->m_Registry.get<Box2DWorldComponent>(m_Scene->m_SceneEntity).World->SetGravity({ 0.0f, gravity });
    }

    float Physics2D::GetGravity()
    {
        return m_Scene->m_Registry.get<Box2DWorldComponent>(m_Scene->m_SceneEntity).World->GetGravity().y;
    }
}