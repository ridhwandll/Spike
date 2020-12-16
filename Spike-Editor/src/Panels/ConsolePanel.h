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
#include <vector>
#include <string>
#include "../Panels/Panel.h"
#include "../FontAwesome.h"

namespace Spike
{
    class Console : public Panel
    {
    public:
        ~Console();

        enum class LogLevel
        {
            LVL_INFO, LVL_DEBUG, LVL_WARN, LVL_ERROR, LVL_CRITICAL
        };

        static Console* Get();

        void OnImGuiRender();
        void Print(std::string message, LogLevel level = LogLevel::LVL_DEBUG);

        virtual Entity GetSelectedEntity() const override { return m_SelectionContext; }
        virtual Ref<Scene> GetCurrentScene() const override { return m_Context; }
    private:
        Console();
        void ClearLog();

    private:
        static Console* m_Console;
        std::vector<std::pair<LogLevel, std::string>> m_Messages{};
        bool m_AutoScrollingEnabled = true;
    };
}