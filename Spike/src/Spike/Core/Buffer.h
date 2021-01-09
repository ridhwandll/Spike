/*****************************************************************************/
/*                        Spike SourceCode                                   */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: None                                                       */
/*                                                                           */
/*   Licensed under the Apache License, Version 2.0 (the "License");         */
/*   you may not use this file except in compliance with the License.        */
/*   You may obtain a copy of the License at                                 */
/*                                                                           */
/*       http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                           */
/*   Unless required by applicable law or agreed to in writing, software     */
/*   distributed under the License is distributed on an "AS IS" BASIS,       */
/*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*   See the License for the specific language governing permissions and     */
/*   limitations under the License.                                          */
/*****************************************************************************/
#pragma once
#include "Log.h"
#include "Base.h"

namespace Spike
{
    struct Buffer
    {
        byte* Data;
        uint32_t Size;

        Buffer()
            : Data(nullptr), Size(0) {}

        Buffer(byte* data, uint32_t size)
            : Data(data), Size(size) {}

        static Buffer Copy(void* data, uint32_t size)
        {
            Buffer buffer;
            buffer.Allocate(size);
            memcpy(buffer.Data, data, size);
            return buffer;
        }

        void Allocate(uint32_t size)
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
        T& Read(uint32_t offset = 0)
        {
            return *(T*)(Data + offset);
        }

        void Write(void* data, uint32_t size, uint32_t offset = 0)
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

        uint32_t GetSize() const { return Size; }
    };
}
