#include "AdranPreCompile.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Adran
{
	static const uint32_t s_maxFramebufferSize = 8192;

	namespace Utils
	{
		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}
			return false;
		}

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static GLenum TextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			AR_CORE_ASSERT(false, "没有该TextureFormat")
			return 0;
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
	{
		m_specification = spec;
		//读取多个attaChment
		{
			for (auto speci : m_specification.attachments.attachments)
			{
				if (!Utils::IsDepthFormat(speci.textureFormat))
					m_colorAttachmentSpecifications.emplace_back(speci);
				else
					m_depthAttachmentSpecifications = speci;
			}
		}
		Modify();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_renderID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Modify()
	{
		if (m_renderID)
		{
			//删除framebuffer
			glDeleteFramebuffers(1, &m_renderID);
			//删除原来的纹理
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);

			m_colorAttachments.clear();
			m_depthAttachment = 0;
		}
		//创建framebuffer
		{
			glCreateFramebuffers(1, &m_renderID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);
		}

		bool multisample = m_specification.Samples > 1;

		//创建并绑定ColorTextrue
		if (m_colorAttachmentSpecifications.size())
		{
			m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());

			//创建ColorAttachment，并绑定
			for (size_t i = 0; i < m_colorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_colorAttachments[i]);
				switch (m_colorAttachmentSpecifications[i].textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_colorAttachments[i], m_specification.Samples, GL_RGBA8, GL_RGBA, m_specification.width, m_specification.height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_colorAttachments[i], m_specification.Samples, GL_R32I, GL_RED_INTEGER, m_specification.width, m_specification.height, i);
					break;
				}
			}
		}

		//创建depthAttachment，并绑定
		if (m_depthAttachmentSpecifications.textureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_depthAttachment, 1);
			Utils::BindTexture(multisample, m_depthAttachment);
			switch (m_depthAttachmentSpecifications.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_depthAttachment, m_specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
				break;
			}
		}

		//DRAWBuffer
		if (m_colorAttachments.size() > 1)
		{
			AR_CORE_ASSERT(m_colorAttachments.size() <= 4, "FrameBuffer的图片组件个数超过了4!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else if (m_colorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		AR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer没有准备好!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::ReSize(uint32_t width, uint32_t  height)
	{
		if (width == 0 || height == 0 || width > s_maxFramebufferSize || height > s_maxFramebufferSize)
		{
			AR_CORE_WARN("尝试rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_specification.width = width;
		m_specification.height = height;
		Modify();
	}

	int OpenGLFrameBuffer::GetPixel(uint32_t attackmentIndex, uint32_t x, uint32_t y)const
	{
		AR_CORE_ASSERT(attackmentIndex < m_colorAttachments.size(), "GetPixel的时候, attachment index 超出范围");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attackmentIndex);
		int pixel;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
		return pixel;
	}
	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		AR_CORE_ASSERT(attachmentIndex < m_colorAttachments.size(), "在ClearAttachment时，attachmentIncex下标越界!");
		auto& spec = m_colorAttachmentSpecifications[attachmentIndex];


		glClearTexImage(m_colorAttachments[attachmentIndex], 0, Utils::TextureFormatToGL(spec.textureFormat), GL_INT, &value);

	}
}