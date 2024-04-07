#pragma once
#include "../Camera/OrthographicCamera.h"
#include "../RenderObject/Model/Model.h"
#include "../Camera/EditorCamera.h"
#include "../RenderObject/Light.h"
namespace Adran
{

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera, const Ref<Light>& light, const glm::vec3& viewPos);
		static void EndScene();
		static void Flush();
		static void DrawVertexArray(const Ref<VertexArray>& vertexArray);
		
		static void DrawModelEntity(const glm::mat4& transform, int EntityID, const Ref<Model>& model);
	};
}