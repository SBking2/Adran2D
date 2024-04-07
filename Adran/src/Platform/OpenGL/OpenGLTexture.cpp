#include "AdranPreCompile.h"
#include "OpenGLTexture.h"

#include "stb_image.h"


namespace Adran
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path):m_path(path)
	{
		m_path = path;

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		//AR_CORE_ASSERT(data, "图片加载失败!");
		m_width = width;
		m_height = height;
		m_channels = channels;

		Init();
		SetData(data, m_width * m_height * m_channels);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data)
		:m_width(width), m_height(height)
	{
		Init();
		if (data)
		{
			SetData(data, m_width * m_height * m_channels);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_renderID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		AR_CORE_ASSERT(size == m_width * m_height * m_channels, "data must be entire texture!");
		glTextureSubImage2D(m_renderID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Init()
	{
		if (m_channels == 4)
		{
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
		}
		else if (m_channels == 3)
		{
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
		}

		AR_CORE_ASSERT(m_internalFormat & m_dataFormat, "当前图片格式不支持!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderID);
		glTextureStorage2D(m_renderID, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_renderID);
	}

}