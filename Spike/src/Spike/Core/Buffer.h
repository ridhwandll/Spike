//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Log.h"
#include "Base.h"

namespace Spike
{
    struct Buffer
    {
        byte* Data;
        Uint Size;

        Buffer()
            : Data(nullptr), Size(0) {}

        Buffer(byte* data, Uint size)
            : Data(data), Size(size) {}

        static Buffer Copy(void* data, Uint size)
        {
            Buffer buffer;
            buffer.Allocate(size);
            memcpy(buffer.Data, data, size);
            return buffer;
        }

        void Allocate(Uint size)
        {
            delete[] Data;
            Data = nullptr;

            if (size == 0) return;

            Data = new byte[size];
            Size = size;
        }

        void ZeroInitialize()
        {
            if (Data)
                memset(Data, 0, Size);
        }

        template<typename T>
        T& Read(Uint offset = 0)
        {
            return *(T*)(Data + offset);
        }

        void Write(void* data, Uint size, Uint offset = 0)
        {
            SPK_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
            memcpy(Data + offset, data, size);
        }

        operator bool()
        {
            return Data;
        }

        operator bool() const
        {
            return Data;
        }

        byte& operator[](int index)
        {
            return Data[index];
        }

        byte operator[](int index) const
        {
            return Data[index];
        }

        template<typename T>
        T* As()
        {
            return (T*)Data;
        }

        Uint GetSize() const { return Size; }
    };
}
