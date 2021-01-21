/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Ref
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Core/Log.h"
#include <stdint.h>

namespace Spike
{
//#define ENABLE_REF_LOGGING
#ifdef ENABLE_REF_LOGGING
    #define SPK_REF_LOG_INCREMENT(...) SPK_CORE_LOG_WARN(__VA_ARGS__)
    #define SPK_REF_LOG_DECREMENT(...) SPK_CORE_LOG_INFO(__VA_ARGS__)
#else
    #define SPK_REF_LOG_INCREMENT(...)
    #define SPK_REF_LOG_DECREMENT(...)
#endif


    class RefCounted
    {
    public:
        void IncRefCount() const
        {
            m_RefCount++;
            SPK_REF_LOG_INCREMENT("REF incremented!");
        }
        void DecRefCount() const
        {
            m_RefCount--;
            SPK_REF_LOG_DECREMENT("REF decremented!");
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

    /*Useful typedefs*/
    typedef uint32_t RendererID;

    // TODO: WeakRef
}