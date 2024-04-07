#include "AdranPreCompile.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Adran
{
	static GLenum GetOpenGLType(const ShaderDataType& type)
	{
		switch (type)
		{
		case ShaderDataType::NONE: AR_CORE_ASSERT(false, "Shader Data Type is NONE !"); return 0;
			break;
		case ShaderDataType::Float:       return GL_FLOAT;
			break;
		case ShaderDataType::Float2:      return GL_FLOAT;
			break;
		case ShaderDataType::Float3:      return GL_FLOAT;
			break;
		case ShaderDataType::Float4:      return GL_FLOAT;
			break;
		case ShaderDataType::Mat3:        return GL_FLOAT;
			break;
		case ShaderDataType::Mat4:        return GL_FLOAT;
			break;
		case ShaderDataType::Int:         return GL_INT;
			break;
		case ShaderDataType::Int2:        return GL_INT;
			break;
		case ShaderDataType::Int3:        return GL_INT;
			break;
		case ShaderDataType::Int4:        return GL_INT;
			break;
		case ShaderDataType::Bool:        return GL_BOOL;
			break;
		}
		AR_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_renderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_renderID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_renderID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		AR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		Bind();
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.dataType)
			{
			case ShaderDataType::Int:
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(
					index,
					element.GetComponentCount(),
					GetOpenGLType(element.dataType),
					layout.GetStride(),
					(const void*)element.offset
				);
				break;
			default:
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index,
					element.GetComponentCount(),
					GetOpenGLType(element.dataType),
					element.isNormalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset
				);
				index++;
				break;
			}
		}
		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_indexBuffer = indexBuffer;
	}

	const std::vector<std::shared_ptr<Adran::VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_vertexBuffers;
	}

	const std::shared_ptr<Adran::IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}

}