#pragma once
#include <entt.hpp>
#include<unordered_map>
#include<functional>

#include"Adran\Core\TimeStep.h"
#include "Adran\Renderer\Camera\EditorCamera.h"
#include "Adran/Core/UUID.h"

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
		Entity CreateEntityWithID(uint64_t uuid, std::string name);
		void DestroyEntity(Entity entity);
		entt::registry& Reg() { return m_registry; };
		void OnUpdate(TimeStep ts);
		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void SetViewport(uint32_t width, uint32_t height);
		inline SceneState GetSceneState() { return  m_sceneState; }
		inline void SetSceneState(SceneState state) { m_sceneState = state; }
		inline const glm::vec2& GetViewportSize() { m_viewportSize; }
		inline void SetViewportSize(const glm::vec2& size) { m_viewportSize = size; }

		void FuncOnAllEntity(std::function<void(Entity)> function);
		inline std::vector<entt::entity> GetEntityOrderList() { return m_entityOrderList; }
	public:
		static Ref<Scene> Copy(Ref<Scene> scene);
	private:
		std::unordered_map<UUID, entt::entity> m_entityMap;

		std::vector<entt::entity> m_entityOrderList;
		//registry有默认构造函数
		entt::registry m_registry;
		glm::vec2 m_viewportSize = { 0.0f, 0.0f };
		SceneState m_sceneState = SceneState::Editor;
		friend class Entity;
	};
}