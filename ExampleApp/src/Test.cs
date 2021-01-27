using System;
using Spike;

class Test : Entity
{
    public int Speed = 2;
    private TransformComponent m_Transform;
    private Entity m_Entity;

    public void OnCreate()
    {
        m_Entity = FindEntityByTag("Player");
        m_Transform = m_Entity.GetComponent<TransformComponent>();
    }

    public void OnUpdate(float ts)
    {
        Vector3 m_Translation = m_Transform.Translation;

        /* Translation */
        if (Input.IsKeyPressed(KeyCode.D))
        {
            m_Translation.X += Speed * ts;
        }

        if (Input.IsKeyPressed(KeyCode.A))
        {
            m_Translation.X -= Speed * ts;
        }

        if (Input.IsKeyPressed(KeyCode.W))
        {
            m_Translation.Y += Speed * ts;
        }

        if (Input.IsKeyPressed(KeyCode.S))
        {
            m_Translation.Y -= Speed * ts;
        }

        m_Transform.Translation = m_Translation;
    }
}
