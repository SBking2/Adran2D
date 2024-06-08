#include "AdranPreCompile.h"
#include "AnimationSerialize.h"
#include "Adran/Utlis/SerializeUtils.h"


namespace Adran
{
	


	void AnimationSerialize::SerializeAnimation(const Ref<Animation>& animation, const std::string& path)
	{

		YAML::Emitter out;

		out << YAML::BeginMap;
		
			out << YAML::Key << "Animation" << YAML::Value << "Adran Animation";
			out << YAML::Key << "Animation_Duration" << YAML::Value << animation->m_duration;
			out << YAML::Key << "Animation_Samples" << YAML::Value << animation->m_samples;
			out << YAML::Key << "KeyFrame" << YAML::BeginMap;
				out << YAML::Key << "Transform_Key" << YAML::BeginMap;

					out << YAML::Key << "Position_Key" << YAML::BeginMap;
						//PositionKey
						for (const auto& it : animation->m_positionLineMap)
						{
							out << YAML::Key << it.first << YAML::Value << it.second;
						}
					out  << YAML::EndMap;
					out << YAML::Key << "Rotation_Key" << YAML::BeginMap;
						//RotationKey
						for (const auto& it : animation->m_rotationLineMap)
						{
							out << YAML::Key << it.first << YAML::Value << it.second;
						}
					out << YAML::EndMap;
					out << YAML::Key << "Scale_Key" << YAML::BeginMap;
						//ScaleKey
						for (const auto& it : animation->m_scaleLineMap)
						{
							out << YAML::Key << it.first << YAML::Value << it.second;
						}
					out << YAML::EndMap;
				out << YAML::EndMap;

				out << YAML::Key << "Texture_Key" << YAML::BeginMap;
					//TextureKey
					for (const auto& it : animation->m_texturePathLineMap)
					{
						out << YAML::Key << it.first << YAML::Value << it.second;
					}
				out << YAML::EndMap;

			out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}
	Ref<Animation> AnimationSerialize::DeSerializeAnimation(const std::string& path)
	{
		std::ifstream fin(path);
		std::stringstream strStream;
		//读取全部内容
		strStream << fin.rdbuf();
		
		YAML::Node data = YAML::Load(strStream.str());
		
		if (!data["Animation"])
		{
			AR_CORE_ERROR("反序列化动画失败!");
		}

		std::string animationName = data["Animation"].as<std::string>();
		AR_CORE_INFO("反序列化动画{0}", animationName);

		auto duration = data["Animation_Duration"].as<float>();
		auto samples = data["Animation_Samples"].as<uint32_t>();

		Ref<Animation> animation = CreateRef<Animation>(duration, samples);

		auto keyFrame = data["KeyFrame"];
		auto Transform = keyFrame["Transform_Key"];

		auto position = Transform["Position_Key"];
		auto rotation = Transform["Rotation_Key"];
		auto scale = Transform["Scale_Key"];

		auto texture = keyFrame["Texture_Key"];


		for (int i = 0; i < samples; i++)
		{
			if (position[i])
			{
				animation->AddPositionKeyFrame(i, position[i].as<glm::vec2>());
			}
			if (rotation[i])
			{
				animation->AddRotationKeyFrame(i, rotation[i].as<float>());
			}
			if (scale[i])
			{
				animation->AddScaleKeyFrame(i, scale[i].as<glm::vec2>());
			}
			if (texture[i])
			{
				animation->AddTextureKeyFrame(i, texture[i].as<std::string>());
			}
		}
		return animation;
	}
}