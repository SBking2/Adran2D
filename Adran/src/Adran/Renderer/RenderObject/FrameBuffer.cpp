#include"AdranPreCompile.h"
#include"FrameBuffer.h"

#include"Platform\OpenGL\OpenGLFrameBuffer.h"
#include "../Renderer.h"

namespace Adran
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE:		AR_CORE_ASSERT(false, "äÖÈ¾APIÎªNONE£¡"); break;
		case RenderAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>(spec); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}