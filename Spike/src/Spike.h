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



//For use by Spike Applications
#include "Spike/Debug/Instrumentor.h"
#include "Spike/Core/Application.h"
#include "Spike/Core/Layer.h"
#include "Spike/Core/Log.h"
#include "Spike/Utility/Random.h"

#include "Spike/Core/Timestep.h"

#include "Spike/Core/Input.h"
#include "Spike/Core/KeyCodes.h"
#include "Spike/Core/MouseCodes.h"

#include "Spike/Renderer/OrthographicCameraController.h"
#include "Spike/ImGui/ImGuiLayer.h"
#include "Spike/Scene/Scene.h"
#include "Spike/Scene/Components.h"
#include "Spike/Scene/Entity.h"
#include "Spike/Scene/ScriptableEntity.h"

//******Renderer**********
#include "Spike/Renderer/Renderer.h"
#include "Spike/Renderer/Renderer2D.h"
#include "Spike/Renderer/RenderCommand.h"

#include "Spike/Renderer/Buffer.h"
#include "Spike/Renderer/Shader.h"
#include "Spike/Renderer/Framebuffer.h"
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/SubTexture2D.h"
#include "Spike/Renderer/VertexArray.h"

#include "Spike/Renderer/OrthographicCamera.h"

