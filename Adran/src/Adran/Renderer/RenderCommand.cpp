#include "AdranPreCompile.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Adran
{
	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI;
}