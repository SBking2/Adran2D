#include "AdranPreCompile.h"
#include "Texture.h"

#include "../Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Adran
{
	
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE:		AR_CORE_ASSERT(false, "äÖÈ¾APIÎªNONE£¡"); break;
		case RenderAPI::API::OpenGL:	return  CreateRef<OpenGLTexture2D>(path); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::CreateFlatColor(uint32_t width, uint32_t height, void* data)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE:		AR_CORE_ASSERT(false, "äÖÈ¾APIÎªNONE£¡"); break;
		case RenderAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height, data); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}


}