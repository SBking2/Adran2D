#pragma once

#include "RenderCommand.h"
#include "Camera/OrthographicCamera.h"

#include "RenderObject/Shader.h"

namespace Adran {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		
		static void OnWindowResize(uint32_t width, uint32_t height)
		{
			RenderCommand::SetViewport(0, 0, width, height);
		}

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
	public:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}