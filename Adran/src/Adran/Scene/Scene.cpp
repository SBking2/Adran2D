#include "AdranPreCompile.h"

#include "Scene.h"	
#include "Component.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include "Adran\Renderer\Render2D\Renderer2D.h"
#include "Adran\Renderer\Render3D\Renderer3D.h"
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
		Entity entity = { m_registry.create(), this };
		entity.AddComponent<IDComponent>(UUID());
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<NameComponent>(name);
		return entity;
	}
	
	void Scene::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}
	
	//void Scene::OnUpdate(TimeStep ts)
	//{
	//	//Update Script

	//	auto& script = m_registry.view<ScriptComponent>();
	//	for (auto entity : script)
	//	{
	//		auto& sc = script.get<ScriptComponent>(entity);
	//		if (sc.instance == nullptr)
	//		{
	//			sc.instance = sc.InitalScript();
	//			if (sc.instance != nullptr)
	//			{
	//				sc.instance->m_entity = Entity{ entity, this };
	//				sc.instance->OnCreate();
	//			}
	//		}
	//		sc.instance->OnUpdate(ts);
	//	}



	//	SceneCamera* mainCamera = nullptr;
	//	glm::mat4 cameraTransform;
	//	auto view = m_registry.view<TransformComponent, CameraComponent>();
	//	for (auto entity : view)
	//	{
	//		auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

	//		if (camera.isPrimary == true)
	//		{
	//			mainCamera = &camera.camera;
	//			cameraTransform = (const glm::mat4 &)transform;
	//			break;
	//		}
	//	}

	//	//Render
	//	{
	//		if (mainCamera != nullptr)
	//		{
	//			Renderer2D::BeginScene(*mainCamera, cameraTransform);

	//			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
	//			for (auto entity : group)
	//			{
	//				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
	//				Renderer2D::DrawQuad((const glm::mat4&)transform, sprite.color, sprite.texture);
	//			}

	//			Renderer2D::EndScene();
	//		}
	//	}
	//}

	//void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	//{
	//	//Render
	//	{
	//		Renderer2D::BeginScene(camera);

	//		auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
	//		for (auto entity : group)
	//		{
	//			auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
	//			Renderer2D::DrawQuadEntity((const glm::mat4&)transform, sprite.color, (int)entity, sprite.texture);
	//		}

	//		Renderer2D::EndScene();
	//	}
	//	
	//}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera, const Ref<Light>& light)
	{
		{
			Renderer3D::BeginScene(camera, light, camera.GetPosition());
			
			auto group = m_registry.group<TransformComponent>(entt::get<ModelComponent>);
			for (auto entity : group)
			{
				auto& [transform, model] = group.get<TransformComponent, ModelComponent>(entity);
				if (m_registry.all_of<AnimationComponent>(entity))
				{
					auto& ac = m_registry.get<AnimationComponent>(entity);
					if (ac.animation != nullptr)
					{
						ac.animation->OnAnimation(model.model, ts);
					}
				}
				if (model.model == nullptr)
				{
					uint32_t color = 0xff00ff00;
					auto flat = Texture2D::Create("assets/textures/SBKING.png");
					model.model = Model::CreateDefaultModel(flat);
				}
				else
				{
					Renderer3D::DrawModelEntity((const glm::mat4&)transform, (int)entity, model);
				}
			}

			Renderer3D::EndScene();
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
}