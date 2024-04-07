#pragma once
#include<glm/glm.hpp>
namespace Adran
{
	//framebuffer的图片格式
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};
	//frameBuffer图片
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};
	//frameBuffer组件类，目前存储所有图片
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};
	
	struct FrameBufferSpecification
	{
		uint32_t width, height;
		FramebufferAttachmentSpecification attachments;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void ReSize(uint32_t width, uint32_t height) = 0;
		virtual const FrameBufferSpecification& GetSpec() const = 0;
		virtual int GetPixel(uint32_t attackmentIndex, uint32_t x, uint32_t y)const = 0;
		virtual uint32_t GetColorAttachmentRenderID(uint32_t index) const = 0;
		virtual uint32_t GetDepthAttachmentRenderID() const = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}