#include "AdranPreCompile.h"
#include "VertexArray.h"

#include "../Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Adran
{

	VertexArray::~VertexArray()
	{

	}

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE:		AR_CORE_ASSERT(false, "äÖÈ¾APIÎªNONE£¡"); break;
		case RenderAPI::API::OpenGL:	return  std::make_shared <OpenGLVertexArray>(); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}