#pragma once

#include"Scene.h"
#include"yaml-cpp\yaml.h"
namespace Adran
{
	class SceneSerialize
	{
	public:
		SceneSerialize(const Ref<Scene>& scene);
		void Serialize(const std::string& path);
		bool DeSerialize(const std::string& path);
	private:
		void SerialEntity(YAML::Emitter& out, Entity entity);
	private:
		Ref<Scene> m_scene;
	};

}