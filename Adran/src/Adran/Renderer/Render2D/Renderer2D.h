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
		static void BeginScene(const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuadEntity(const glm::mat4& transform, const glm::vec4& color, int EntityID, const Ref<Texture2D>& texture = nullptr);
		static void DrawQuadEntity(const glm::mat4& transform, const glm::vec4& color, int EntityID, const glm::vec2& minTexCoord, const glm::vec2& maxTexCoord
			, const Ref<Texture2D>& texture = nullptr);


	private:
		static void StartBatch();
	};
}