#pragma once

#include "Adran/Core/TimeStep.h"
#include "Model.h"

#include<unordered_map>
#include <vector>
#include <string>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


//目前只打算做一个动画配一个模型
namespace Adran
{
	class Animation 
	{
		struct BoneTransformTrack
		{
			//在某个时间点的boneTransform，相当于关键帧
			std::vector<float> positionTimestamps = {};
			std::vector<float> rotationTimestamps = {};
			std::vector<float> scaleTimestamps    = {};

			std::vector<glm::vec3> positions = {};
			std::vector<glm::quat> rotations = {};
			std::vector<glm::vec3> scales    = {};

		};
	public:
		Animation(const std::string& filePath);

		void OnAnimation(Ref<Model>& model, TimeStep ts);
	private:
		void updateAnimation(std::unordered_map<std::string, Ref<Model::Bone>>& boneMap, Ref<Model::Bone>& bone, TimeStep ts, glm::mat4& parentTransform);
	private:
		void LoadAnimation(const std::string& filePath);
		std::pair<uint32_t, float> getTimeFraction(std::vector<float>& times, float& dt);
	private:
		float m_timeContainer = 0.0f;
		float m_duration = 0.0f;
		float m_ticksPerSecond = 1.0f;
		std::unordered_map<std::string, BoneTransformTrack> m_boneTransformsMap;
	};
}