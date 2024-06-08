#include "AdranPreCompile.h"

#include "Scene.h"	
#include "Component.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include "Adran/Core/AssetsManager.h"
#include "Adran\Renderer\Render2D\Renderer2D.h"
//#include "Adran\Renderer\Render3D\Renderer3D.h"
#include "Adran/Renderer/Camera/SceneCamera.h"

namespace Adran
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}
	Entity Scene::CreateEntity(std::string name)
	{	
		return CreateEntityWithID(UUID(), name);
	}
	Entity Scene::CreateEntityWithID(uint64_t uuid, std::string name)
	{
		Entity entity = { m_registry.create(), this };
		entity.AddComponent<IDComponent>(UUID(uuid));
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<NameComponent>(name);
		entity.AddComponent<RenderNumComponent>();
		m_entityMap[UUID(uuid)] = entity;
		m_entityOrderList.push_back(entity);
		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}
	
	void Scene::OnUpdate(TimeStep ts)
	{
		//Update Script

		auto& script = m_registry.view<ScriptComponent>();
		for (auto entity : script)
		{
			auto& sc = script.get<ScriptComponent>(entity);
			if (sc.instance == nullptr)
			{
				sc.instance = sc.InitalScript();
				if (sc.instance != nullptr)
				{
					sc.instance->m_entity = Entity{ entity, this };
					sc.instance->OnCreate();
				}
			}
			sc.instance->OnUpdate(ts);
		}



		SceneCamera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.isPrimary == true)
			{
				mainCamera = &camera.camera;
				cameraTransform = (const glm::mat4 &)transform;
				break;
			}
		}

		//Render
		{
			if(mainCamera == nullptr)
				Renderer2D::BeginScene(cameraTransform);
			else
				Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				auto tempTransform = transform;
				if (m_sceneState == SceneState::Play)
				{
					auto& AnimEntity = Entity{ entity, this };
					if (AnimEntity.HasComponent<AnimationComponent>())
					{
						auto& ac = AnimEntity.GetComponent<AnimationComponent>();
						ac.animation->OnUpdate(ts);

						tempTransform.position += ac.animation->GetPosition();
						tempTransform.rotation += ac.animation->GetRotation();
						tempTransform.scale *= ac.animation->GetScale();
						std::string spritePath = ac.animation->GetTexturePath();
						if (!spritePath.empty())
							sprite.texture = AssetsManager::GetInstance()->GetTexture2D(spritePath);
					}
				}
				Renderer2D::DrawQuadEntity((const glm::mat4&)tempTransform, sprite.color, (int)entity, sprite.minTexCoords, sprite.maxTexCoords, sprite.texture);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		//Render
		{
			Renderer2D::BeginScene(camera);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawQuadEntity((const glm::mat4&)transform, sprite.color, (int)entity, sprite.texture);
			}

			Renderer2D::EndScene();
		}
		
	}
	
	//为外界提供一个接口，使得外界函数能够遍历orderList的每个元素并作操作，ETFunction的形参为Entity

	void Scene::FuncOnAllEntity(std::function<void(Entity)> function)
	{
		for (auto& entity : m_entityOrderList)
		{
			function(Entity{entity, this});
		}
	}


	void Scene::SetViewport(uint32_t width, uint32_t height)
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			camera.camera.SetViewPort(width, height);
		}
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).UID);

					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}


	Ref<Scene> Scene::Copy(Ref<Scene> scene)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_viewportSize[0] = scene->m_viewportSize[0];
		newScene->m_viewportSize[1] = scene->m_viewportSize[1];

		auto& srcSceneRegistry = scene->m_registry;
		auto& dstSceneRegistry = newScene->m_registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scen
		auto idView = srcSceneRegistry.view<IDComponent>();

		scene->FuncOnAllEntity([&](Entity entity)
			{
				auto e = entity.Reg();
				UUID uuid = srcSceneRegistry.get<IDComponent>(e).UID;
				const auto& name = srcSceneRegistry.get<NameComponent>(e).name;
				Entity newEntity = newScene->CreateEntityWithID(uuid, name);
				enttMap[uuid] = (entt::entity)newEntity;
			});

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}
}