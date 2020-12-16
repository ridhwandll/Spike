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
#include "Spike/Scene/Scene.h"
#include "Spike/Scene/Entity.h"

namespace Spike
{
    class Panel
    {
    public:
        virtual ~Panel() = default;

        static Panel* Get();

        virtual Entity GetSelectedEntity() const { return m_SelectionContext; };
        virtual Ref<Scene> GetCurrentScene() const { return m_Context; };

    protected:
        Panel() = default;
        static Panel* m_Instance;
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}
