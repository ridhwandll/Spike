using Spike;

class Player : Entity
{
    public float Force = 0.3f;

    private RigidBody2DComponent m_Rb2d;
    public void Start()
    {
        m_Rb2d = GetComponent<RigidBody2DComponent>();
    }

    public void Update(float ts)
    {
        float movementForce = Force;

        if (Input.IsKeyPressed(KeyCode.D))
            m_Rb2d.ApplyLinearImpulse(new Vector2(movementForce, 0), new Vector2(), true);

        else if (Input.IsKeyPressed(KeyCode.A))
            m_Rb2d.ApplyLinearImpulse(new Vector2(-movementForce, 0), new Vector2(), true);

        else if (Input.IsKeyPressed(KeyCode.S))
            m_Rb2d.ApplyLinearImpulse(new Vector2(0, -movementForce), new Vector2(), true);

        if (Input.IsKeyPressed(KeyCode.W))
            m_Rb2d.ApplyLinearImpulse(new Vector2(0, movementForce), new Vector2(), true);
    }

    public void FixedUpdate(float fixedTimestep)
    {
    }
}