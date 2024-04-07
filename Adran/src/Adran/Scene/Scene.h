#pragma once
#include <entt.hpp>
#include"Adran\Core\TimeStep.h"
#include "Adran\Renderer\Camera\EditorCamera.h"
#include "Adran/Renderer/RenderObject/Light.h"
namespace Adran
{
	class Entity;
	
	class Scene
	{
	public:
		enum class SceneState
		{
			Editor = 0,
			Play = 1
		};
	public:
		Scene();
		~Scene();
		Entity CreateEntity(std::string name = "GameObject");
		void DestroyEntity(Entity entity);
		entt::registry& Reg() { return m_registry; };
		//void OnUpdate(TimeStep ts);
		//void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void OnUpdateEditor(TimeStep ts, EditorCamera& camera, const Ref<Light>& light);
		void SetViewport(uint32_t width, uint32_t height);
	public: 
		SceneState sceneState = SceneState::Editor;
	private:
		//registry有默认构造函数
		entt::registry m_registry;

		friend class Entity;
	};
}