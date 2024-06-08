#include "AdranPreCompile.h"
#include "SceneSerialize.h"

#include "Entity.h"
#include"Component.h"
#include "Adran/Core/AssetsManager.h"
#include "Adran/Utlis/SerializeUtils.h"

#include <glm/gtx/quaternion.hpp>


namespace Adran
{

	SceneSerialize::SceneSerialize(const Ref<Scene>& scene):m_scene(scene)
	{

	}

	void SceneSerialize::Serialize(const std::string& path)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Adran Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		auto list = m_scene->GetEntityOrderList();
		for (auto entity : list)
		{
			SerialEntity(out, Entity{ entity, m_scene.get() });
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;


		std::ofstream fout(path);	
		fout << out.c_str();
	}
	bool SceneSerialize::DeSerialize(const std::string& path)
	{
		std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;
		std::string sceneName = data["Scene"].as<std::string>();
		AR_CORE_INFO("反序列化场景:{0}", sceneName);

		std::vector<Entity> Entityes;

		auto entities = data["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				//Name
				auto nc = entity["NameComponent"];
				std::string name = nc["Name"].as<std::string>();
				Entity gameObject = gameObject = m_scene->CreateEntityWithID(uuid, name);

					//Tranform
				auto transform = entity["TransformComponent"];

				{
					auto& tc = gameObject.GetComponent<TransformComponent>();
					tc.position = transform["position"].as<glm::vec2>();
					tc.rotation = transform["rotation"].as<float>();
					tc.scale = transform["scale"].as<glm::vec2>();
				}

				auto camera = entity["CameraComponent"];

				if (camera)
				{
					auto& cc = gameObject.AddComponent<CameraComponent>();

					cc.camera.SetOrthSize(camera["OrthSize"].as<float>());
					cc.camera.SetOrthNear(camera["OrthNear"].as<float>());
					cc.camera.SetOrthFar(camera["OrthFar"].as<float>());
				}

				auto sprite = entity["SpriteComponent"];

				if (sprite)
				{
					auto& sc = gameObject.AddComponent<SpriteComponent>();
					if (sprite["TexturePath"])
					{
						sc.texture = AssetsManager::GetInstance()->GetTexture2D(sprite["TexturePath"].as<std::string>());
					}
					sc.color = sprite["Color"].as<glm::vec4>();
				}

			}
		}

		m_scene->Reg().each([&](entt::entity entity)
			{
				Entity en = Entity{ entity, m_scene.get() };
				AR_ERROR("DeSerialize {0}", en.GetComponent<NameComponent>().name);
			});
	}

	void SceneSerialize::SerialEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;//Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().UID;

		if (entity.HasComponent<NameComponent>())
		{
			out << YAML::Key << "NameComponent";
			out << YAML::BeginMap;//Sprite Component

			auto& nc = entity.GetComponent<NameComponent>();
			out << YAML::Key << "Name" << YAML::Value << nc.name;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;//Sprite Component

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "position" << YAML::Value << tc.position;
			out << YAML::Key << "rotation" << YAML::Value << tc.rotation;
			out << YAML::Key << "scale" << YAML::Value << tc.scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;//Sprite Component

			auto& cc = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "OrthSize" << YAML::Value << cc.camera.GetOrthSize();
			out << YAML::Key << "OrthNear" << YAML::Value << cc.camera.GetOrthNear();
			out << YAML::Key << "OrthFar" << YAML::Value << cc.camera.GetOrthFar();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;//Sprite Component

			auto& sc = entity.GetComponent<SpriteComponent>();
			if (sc.texture != nullptr)
			{
				out << YAML::Key << "TexturePath" << YAML::Value << sc.texture->GetPath();
			}
			out << YAML::Key << "Color" << YAML::Value << sc.color;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}
}
