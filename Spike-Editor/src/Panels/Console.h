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
#include "../FontAwesome.h"

namespace Spike
{
    class Console
    {
    public:
        ~Console();

        enum class LogLevel
        {
            LVL_INFO, LVL_DEBUG, LVL_WARN, LVL_ERROR 
        };

        static Console* Get();

        void OnImGuiRender();
        void Print(std::string message, LogLevel level = LogLevel::LVL_DEBUG);

    private:
        Console();
        void ClearLog();

    private:
        static Console* m_Console;
        std::vector<std::pair<LogLevel, std::string>> m_Messages{};
        std::string m_IconDebug = ICON_FK_BUG;
        std::string m_IconWarn = ICON_FK_EXCLAMATION_CIRCLE;
        std::string m_IconError = ICON_FK_EXCLAMATION_TRIANGLE;
        std::string m_IconInfo = ICON_FK_INFO_CIRCLE;
    };
}