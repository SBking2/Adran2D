#pragma once
#include "AdranPreCompile.h"

namespace Adran {
	//着色器每个Element数据类型
	enum class ShaderDataType
	{
		NONE = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};
	static uint32_t GetDataBytes(const ShaderDataType & type) {
		switch (type)
		{
		case ShaderDataType::NONE: AR_CORE_ASSERT(false, "Shader Data Type is NONE !"); return 0;
			break;
		case ShaderDataType::Float:       return 4 * 1;
			break;
		case ShaderDataType::Float2:      return 4 * 2;
			break;
		case ShaderDataType::Float3:      return 4 * 3;
			break;
		case ShaderDataType::Float4:      return 4 * 4;
			break;
		case ShaderDataType::Mat3:        return 4 * 3 * 3;
			break;
		case ShaderDataType::Mat4:        return 4 * 4 * 4;
			break;
		case ShaderDataType::Int:         return 4 * 1;
			break;
		case ShaderDataType::Int2:        return 4 * 2;
			break;
		case ShaderDataType::Int3:        return 4 * 3;
			break;
		case ShaderDataType::Int4:        return 4 * 4;
			break;
		case ShaderDataType::Bool:        return 1 * 1;
			break;
		}

		AR_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}
	static uint32_t GetDataCount(const ShaderDataType& type) {
		switch (type)
		{
		case ShaderDataType::NONE: AR_CORE_ASSERT(false, "Shader Data Type is NONE !"); return 0;
			break;
		case ShaderDataType::Float:   return 1;
			break;
		case ShaderDataType::Float2:  return 2;
			break;
		case ShaderDataType::Float3:  return 3;
			break;
		case ShaderDataType::Float4:  return 4;
			break;
		case ShaderDataType::Mat3:    return 3 * 3;
			break;
		case ShaderDataType::Mat4:    return 4 * 4;
			break;
		case ShaderDataType::Int:     return 1;
			break;
		case ShaderDataType::Int2:    return 2;
			break;
		case ShaderDataType::Int3:    return 3;
			break;
		case ShaderDataType::Int4:    return 4;
			break;
		case ShaderDataType::Bool:    return 1;
			break;
		}

		AR_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}
	//BufferElement
	struct BufferElement {
		BufferElement(const std::string& name, const ShaderDataType& type, bool isNormalized = false) {
			this->name = name;
			this->dataType = type;
			this->isNormalized = isNormalized;
			offset = 0;
			size = GetDataBytes(dataType);
		}

		inline uint32_t GetComponentCount() const {
			return GetDataCount(dataType);
		}

		std::string name;
		ShaderDataType dataType;
		uint32_t offset;
		uint32_t size;
		bool isNormalized;
	};
	//BufferLayout
	class BufferLayout
	{
	public:
		BufferLayout(){}
		BufferLayout(const std::initializer_list<BufferElement>& elements) :m_elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement> GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
		inline uint32_t GetStride()const { return m_stride; }
	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}
		std::vector<BufferElement> m_elements;
		uint32_t m_stride;
	};
	//Vertex Buffer
	class VertexBuffer {
	public:
		virtual ~VertexBuffer(){}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout GetLayout()const = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
		static Ref<VertexBuffer> Create(uint32_t count);
	private:
	};
	//Index Buffer
	class IndexBuffer {
	public:
		virtual ~IndexBuffer(){}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		inline virtual uint32_t GetCount() = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	private:
	};
}