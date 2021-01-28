using Spike;

class Test : Entity
{
    public int Speed = 2;
    public float HorizontalForce = 10.0f;
    public float JumpForce = 10.0f;

    private TransformComponent m_Transform;
    private RigidBody2DComponent m_Rb2d;
    public void Start()
    {
        m_Transform = GetComponent<TransformComponent>();
        m_Rb2d = GetComponent<RigidBody2DComponent>();
        AddCollision2DBeginCallback(OnPlayerCollisionBegin);
        AddCollision2DEndCallback(OnPlayerCollisionEnd);
    }

    public void OnPlayerCollisionBegin(float value)
    {
    }
    public void OnPlayerCollisionEnd(float value)
    {
    }


    public void Update(float ts)
    {
        float movementForce = HorizontalForce;

        if (Input.IsKeyPressed(KeyCode.D))
            m_Rb2d.ApplyLinearImpulse(new Vector2(movementForce, 0), new Vector2(), true);
        else if (Input.IsKeyPressed(KeyCode.A))
            m_Rb2d.ApplyLinearImpulse(new Vector2(-movementForce, 0), new Vector2(), true);
        else if (Input.IsKeyPressed(KeyCode.S))
            m_Rb2d.SetLinearVelocity(new Vector2(movementForce, 0));

        if (Input.IsKeyPressed(KeyCode.W))
            m_Rb2d.ApplyLinearImpulse(new Vector2(0, JumpForce), new Vector2(0, 0), true);
    }

    public void FixedUpdate(float ts)
    {
    }
}