#include "AdranPreCompile.h"
#include "Buffer.h"

#include "../Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
namespace Adran
{

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE :		AR_CORE_ASSERT(false, "äÖÈ¾APIÎªNONE£¡"); break;
		case RenderAPI::API::OpenGL :	return CreateRef<OpenGLVertexBuffer>(vertices, size); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE:		AR_CORE_ASSERT(false, "äÖÈ¾APIÎªNONE£¡"); break;
		case RenderAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE :		AR_CORE_ASSERT(false, "äÖÈ¾APIÎªNONE£¡"); break;
		case RenderAPI::API::OpenGL :	return  CreateRef<OpenGLIndexBuffer>(indices, count); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}