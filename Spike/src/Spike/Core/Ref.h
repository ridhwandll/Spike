/*****************************************************************************/
/*                This file is licensed under SPIKE LICENSE                  */
/*                          Spike SourceCode                                 */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: None                                                       */
/*                                                                           */
/*                     SPECIAL NOTE FOR THIS FILE                            */
/* You must NOT copy this file. If you want to use this file, you must retain*/
/*              this licence and this comment block AS IS                    */
/* You must NOT claim that you wrote this file. The "Spike  SourceCode" word */
/*              should be present throughout all the copies                  */
/* You must NOT change this ot edit this file by any means. Namespace should */
/*                 be 'Spike' throughout all the copies                      */
/* You CAN remove the lines marked as '//removeable'. Happy using Spike::Ref!*/
/*****************************************************************************/
#pragma once
#include "Spike/Core/Log.h"
#include <stdint.h>

namespace Spike
{
//#define ENABLE_REF_LOGGING                                          //removeable
#ifdef ENABLE_REF_LOGGING                                             //removeable
    #define SPK_REF_LOG_INCREMENT(...) SPK_CORE_LOG_WARN(__VA_ARGS__) //removeable
    #define SPK_REF_LOG_DECREMENT(...) SPK_CORE_LOG_INFO(__VA_ARGS__) //removeable
#else                                                                 //removeable
    #define SPK_REF_LOG_INCREMENT(...)                                //removeable
    #define SPK_REF_LOG_DECREMENT(...)                                //removeable
#endif                                                                //removeable


    class RefCounted
    {
    public:
        void IncRefCount() const
        {
            m_RefCount++;
            SPK_REF_LOG_INCREMENT("REF incremented!");                //removeable
        }
        void DecRefCount() const
        {
            m_RefCount--;
            SPK_REF_LOG_DECREMENT("REF decremented!");                //removeable
        }

        uint32_t GetRefCount() const { return m_RefCount; }
    private:
        mutable uint32_t m_RefCount = 0; // TODO: atomic
    };

    template<typename T>
    class Ref
    {
    public:
        Ref()
            : m_Instance(nullptr)
        {
        }

        Ref(std::nullptr_t n)
            : m_Instance(nullptr)
        {
        }

        Ref(T* instance)
            : m_Instance(instance)
        {
            static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");

            IncRef();
        }

        template<typename T2>
        Ref(const Ref<T2>& other)
        {
            m_Instance = (T*)other.m_Instance;
            IncRef();
        }

        template<typename T2>
        Ref(Ref<T2>&& other)
        {
            m_Instance = (T*)other.m_Instance;
            other.m_Instance = nullptr;
        }

        ~Ref()
        {
            DecRef();
        }

        Ref(const Ref<T>& other)
            : m_Instance(other.m_Instance)
        {
            IncRef();
        }

        Ref& operator=(std::nullptr_t)
        {
            DecRef();
            m_Instance = nullptr;
            return *this;
        }

        Ref& operator=(const Ref<T>& other)
        {
            other.IncRef();
            DecRef();

            m_Instance = other.m_Instance;
            return *this;
        }

        template<typename T2>
        Ref& operator=(const Ref<T2>& other)
        {
            other.IncRef();
            DecRef();

            m_Instance = other.m_Instance;
            return *this;
        }

        template<typename T2>
        Ref& operator=(Ref<T2>&& other)
        {
            DecRef();

            m_Instance = other.m_Instance;
            other.m_Instance = nullptr;
            return *this;
        }

        operator bool() { return m_Instance != nullptr; }
        operator bool() const { return m_Instance != nullptr; }

        T* operator->() { return m_Instance; }
        const T* operator->() const { return m_Instance; }

        T& operator*() { return *m_Instance; }
        const T& operator*() const { return *m_Instance; }

        T* Raw() { return  m_Instance; }
        const T* Raw() const { return  m_Instance; }

        void Reset(T* instance = nullptr)
        {
            DecRef();
            m_Instance = instance;
        }

        template<typename... Args>
        static Ref<T> Create(Args&&... args)
        {
            return Ref<T>(new T(std::forward<Args>(args)...));
        }
    private:
        void IncRef() const
        {
            if (m_Instance)
                m_Instance->IncRefCount();
        }

        void DecRef() const
        {
            if (m_Instance)
            {
                m_Instance->DecRefCount();
                if (m_Instance->GetRefCount() == 0)
                {
                    delete m_Instance;
                }
            }
        }

        template<class T2>
        friend class Ref;
        T* m_Instance;
    };

    /*Useful typedefs*/                           //removeable
    typedef uint32_t RendererID;                  //removeable

    // TODO: WeakRef                              //removeable
}