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

#ifdef SPK_PLATFORM_WINDOWS

extern Spike::Application* Spike::CreateApplication();

int main(int argc, char** argv)
{
    Spike::Log::Init();
    LE_PROFILE_BEGIN_SESSION("Startup", "SpikeProfile-Startup.json");
    auto app = Spike::CreateApplication();
    LE_PROFILE_END_SESSION();

    LE_PROFILE_BEGIN_SESSION("Runtime", "SpikeProfile-Runtime.json");
    app->Run();
    LE_PROFILE_END_SESSION();

    LE_PROFILE_BEGIN_SESSION("Shutdown", "SpikeProfile-Shutdown.json");
    delete app;
    LE_PROFILE_END_SESSION();
}

#endif // SPK_PLATFORM_WINDOWS
