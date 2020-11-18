#include "lepch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace LightEngine
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}