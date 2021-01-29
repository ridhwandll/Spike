using System;
using Spike;

class Test2 : Entity
{
    private SpriteRendererComponent m_SpriteRenderer;
    public Vector4 m_Color;
    public void Start()
    {
        Random rand = new Random();
        m_SpriteRenderer = GetComponent<SpriteRendererComponent>();
        m_Color = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_SpriteRenderer.SetColor(m_Color);
        AddCollision2DBeginCallback(CollisionOoof);
        AddCollision2DEndCallback(CollisionOoof);
    }

    public void Update(float ts)
    {
        m_SpriteRenderer.SetColor(m_Color);
    }

    public void CollisionOoof(float value)
    {
        Random rand = new Random();
        Vector4 color = new Vector4((float)rand.NextDouble(), (float)rand.NextDouble(), (float)rand.NextDouble(), 1.0f);
        m_Color = color;
    }

}
