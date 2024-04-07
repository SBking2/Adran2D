#pragma once
#include "Adran/Renderer/RenderObject/Buffer.h"

namespace Adran {
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void SetData(const void* data, uint32_t size) override;

		virtual void Bind() const;
		virtual void UnBind() const;


		virtual void SetLayout(const BufferLayout& layout) override;
		virtual const BufferLayout GetLayout()const override;
	private:
		uint32_t m_renderID;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;
		inline uint32_t GetCount() override { return m_count; }
	private:
		uint32_t m_renderID;
		uint32_t m_count;
	};
}