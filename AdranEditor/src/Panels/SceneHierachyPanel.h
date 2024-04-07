#pragma once
#include <Adran.h>
#include "../imgui/imgui.h"
namespace Adran
{
	class SceneHierachyPanel
	{
	public:
		SceneHierachyPanel(const Ref<Scene>& scene);
		~SceneHierachyPanel();

		void SetScene(const Ref<Scene>& scene);

		void OnImGuiRender();
		Entity GetSelectedEntity()
		{
			return m_selectObject;
		}
		void SetSelectedEntity(Entity entity) { m_selectObject = entity; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawProperties(Entity entity);
		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction function);
		void DrawVec3Control(const std::string& label, glm::vec3& value, float resetValue = 0.0f, float columWidth = 100.0f);

		Ref<Scene> m_scene;
		Entity m_selectObject;
	};
}