#include "AdranPreCompile.h"
#include "SceneSerialize.h"

#include "Entity.h"
#include"Component.h"
#include "Adran/Core/AssetsManager.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat& rota)
		{
			Node node;
			node.push_back(rota.x);
			node.push_back(rota.y);
			node.push_back(rota.z);
			node.push_back(rota.w);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rota)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rota.x = node[0].as<float>();
			rota.y = node[1].as<float>();
			rota.z = node[2].as<float>();
			rota.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Adran
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w <<YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& q)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << q.x << q.y << q.z << q.w << YAML::EndSeq;
		return out;
	}

	SceneSerialize::SceneSerialize(const Ref<Scene>& scene):m_scene(scene)
	{

	}

	void SceneSerialize::Serialize(const std::string& path)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Adran Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto& es = m_scene->Reg().view<entt::entity>();
		for (auto& entity : es)
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

		auto entities = data["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				uint32_t uuid = entity["Entity"].as<uint64_t>();

				//Name
				auto nc = entity["NameComponent"];
				std::string name = nc["Name"].as<std::string>();
				
				Entity gameObject = m_scene->CreateEntity(name);
				gameObject.GetComponent<IDComponent>().UID = uuid;
					//Tranform
				auto transform = entity["TransformComponent"];

				{
					auto& tc = gameObject.GetComponent<TransformComponent>();
					tc.position = transform["position"].as<glm::vec3>();
					tc.rotation = transform["rotation"].as<glm::quat>();
					tc.scale = transform["scale"].as<glm::vec3>();
				}

				auto camera = entity["CameraComponent"];

				if (camera)
				{
					auto& cc = gameObject.AddComponent<CameraComponent>();

					cc.camera.SetType((SceneCamera::SceneCameraType)camera["CameraType"].as<int>());
					cc.camera.SetOrthSize(camera["OrthSize"].as<float>());
					cc.camera.SetOrthNear(camera["OrthNear"].as<float>());
					cc.camera.SetOrthFar(camera["OrthFar"].as<float>());

					cc.camera.SetPerFOV(camera["PerFOV"].as<float>());
					cc.camera.SetPerNear(camera["PerNear"].as<float>());
					cc.camera.SetPerFar(camera["PerFar"].as<float>());
				}

				auto model = entity["ModelComponent"];

				if (model)
				{
					auto& mc = gameObject.AddComponent<ModelComponent>();
					if (model["ModelPath"].as<std::string>() == "NULL")
					{

					}
					else
					{
						mc.model = CreateRef<Model>(model["ModelPath"].as<std::string>());
					}
				}

			}
		}

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
			out << YAML::Key << "CameraType" << YAML::Value << (int)cc.camera.GetSceneCameraType();

			out << YAML::Key << "OrthSize" << YAML::Value << cc.camera.GetOrthSize();
			out << YAML::Key << "OrthNear" << YAML::Value << cc.camera.GetOrthNear();
			out << YAML::Key << "OrthFar" << YAML::Value << cc.camera.GetOrthFar();

			out << YAML::Key << "PerFOV" << YAML::Value << cc.camera.GetPerFOV();
			out << YAML::Key << "PerNear" << YAML::Value << cc.camera.GetPerNear();
			out << YAML::Key << "PerFar" << YAML::Value << cc.camera.GetPerFar();
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ModelComponent>())
		{
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;//Sprite Component

			auto& mc = entity.GetComponent<ModelComponent>();
			if (mc.model != nullptr)
			{
				out << YAML::Key << "ModelPath" << YAML::Value << mc.model->GetPath();
			}
			else
			{
				out << YAML::Key << "ModelPath" << YAML::Value << "NULL";
			}

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}
}
