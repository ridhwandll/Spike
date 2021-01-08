/*****************************************************************************/
/*                             Spike SourceCode                              */
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
#include "Spike/Core/Ref.h"
#include "glm/glm.hpp"

namespace Spike
{
    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class Framebuffer : public RefCounted
    {
    public:
        virtual ~Framebuffer() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(const uint32_t width, const uint32_t height) = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual const FramebufferSpecification& GetSpecification() const = 0;
        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}