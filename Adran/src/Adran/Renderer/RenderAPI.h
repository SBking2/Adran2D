#pragma once

//API是使用的RenderAPI类型
//RenderAPI是可以执行RenderCommand的类

#include <glm/glm.hpp>

#include "RenderObject/VertexArray.h"

namespace Adran {
	class RenderAPI {
	public:
		enum class API {
			NONE = 0,
			OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
	private:
		static API s_API;
	public:
		inline static API GetAPI() { return s_API; }
	};
}