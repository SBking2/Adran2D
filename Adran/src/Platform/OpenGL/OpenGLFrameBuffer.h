#pragma once
#include"Adran/Renderer/RenderObject/FrameBuffer.h"

namespace Adran
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void UnBind() override;
		
		void Modify();
		virtual void ReSize(uint32_t width, uint32_t height)override;
		virtual const FrameBufferSpecification& GetSpec()const override { return m_specification; }
		virtual int GetPixel(uint32_t attackmentIndex, uint32_t x, uint32_t y)const override;
		virtual uint32_t GetColorAttachmentRenderID(uint32_t index = 0)const override { AR_CORE_ASSERT(index < m_colorAttachments.size(), "colorAttachment的index已经超出范围!"); return m_colorAttachments[index]; }
		virtual uint32_t GetDepthAttachmentRenderID()const override { return m_depthAttachment; }
		virtual void ClearAttachment(uint32_t attachmentIndex, int value)override;

	private:
		uint32_t m_renderID = 0;
		FrameBufferSpecification m_specification;
		
		//组件
		std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
		std::vector<uint32_t> m_colorAttachments;
		
		FramebufferTextureSpecification m_depthAttachmentSpecifications = FramebufferTextureFormat::None;
		uint32_t m_depthAttachment = 0;
	};
}