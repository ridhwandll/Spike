using System;
using System.Runtime.CompilerServices;

namespace Spike
{
    public class Entity
    {
        private Action<float> m_Collision2DBeginCallbacks;
        private Action<float> m_Collision2DEndCallbacks;

        public ulong ID { get; private set; }

        protected Entity() { ID = 0; }

        internal Entity(ulong id)
        {
            ID = id;
        }

        ~Entity() { }

        public T CreateComponent<T>() where T : Component, new()
        {
            CreateComponent_Native(ID, typeof(T));
            T component = new T();
            component.Entity = this;
            return component;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            return HasComponent_Native(ID, typeof(T));
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
            {
                T component = new T();
                component.Entity = this;
                return component;
            }
            return null;
        }

        public Entity FindEntityByTag(string tag)
        {
            ulong entityID = FindEntityByTag_Native(tag);
            return new Entity(entityID);
        }

        public void AddCollision2DBeginCallback(Action<float> callback)
        {
            m_Collision2DBeginCallbacks += callback;
        }

        public void AddCollision2DEndCallback(Action<float> callback)
        {
            m_Collision2DEndCallbacks += callback;
        }

        private void OnCollision2DBegin(float data)
        {
            if (m_Collision2DBeginCallbacks != null)
                m_Collision2DBeginCallbacks.Invoke(data);
        }

        private void OnCollision2DEnd(float data)
        {
            if (m_Collision2DBeginCallbacks != null)
                m_Collision2DEndCallbacks.Invoke(data);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void CreateComponent_Native(ulong entityID, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Native(ulong entityID, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong FindEntityByTag_Native(string tag);
    }
}
