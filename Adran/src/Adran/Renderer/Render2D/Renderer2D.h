#pragma once
#include "../Camera/OrthographicCamera.h"
#include "../Camera/SceneCamera.h"
#include "../Camera/EditorCamera.h"
#include "../RenderObject/Texture.h"
#include "../RenderObject/SubTexture.h"

namespace Adran
{
	struct Statics
	{
		Statics(uint32_t quadNum)
		{
			QuadNum = quadNum;
		}
		uint32_t drawCallTime = 0;
		float timeStep = 0;
		uint32_t fps = 0;
		uint32_t QuadNum;
	};
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static Statics s_statics;

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const SceneCamera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture = nullptr);
		static void DrawQuadEntity(const glm::mat4& transform, const glm::vec4& color, int EntityID, const Ref<Texture2D>& texture = nullptr);


	private:
		static void StartBatch();
	};
}