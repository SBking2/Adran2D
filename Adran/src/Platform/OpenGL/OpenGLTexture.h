#pragma once

#include "Adran/Renderer/RenderObject/Texture.h"
#include <glad/glad.h>
#include<glm/glm.hpp>
namespace Adran
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height, void* data);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }
		virtual uint32_t GetRenderID() const override { return m_renderID; }
		virtual const std::string& GetPath()const override { return m_path; }
		virtual void SetPath(const std::string& path) { m_path = path; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot = 0) const override;
	private:
		void Init();
	private:
		std::string m_path;
		uint32_t m_renderID;
		uint32_t m_width, m_height, m_channels;
		GLenum m_internalFormat, m_dataFormat;
	};
}