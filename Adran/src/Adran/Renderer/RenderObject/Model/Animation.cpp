#include"AdranPreCompile.h"
#include "Animation.h"

namespace Adran
{

	static inline glm::vec3 assimpToGlmVec3(aiVector3D vec) {
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	static inline glm::quat assimpToGlmQuat(aiQuaternion quat) {
		return glm::quat(quat.x, quat.y, quat.z, quat.w);
	}

	static inline glm::vec3 assimpQuatToGlmVec3(aiQuaternion quat)
	{
		glm::quat quaternion;
		quaternion.x = quat.x;
		quaternion.y = quat.y;
		quaternion.z = quat.z;
		quaternion.w = quat.w;

		return glm::eulerAngles(quaternion);
	}

	Animation::Animation(const std::string& filePath)
	{
		LoadAnimation(filePath);
	}

	void Animation::LoadAnimation(const std::string& filePath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			AR_CORE_ERROR("Assimp报错:{0}", importer.GetErrorString());
			AR_CORE_ASSERT(false, "加载动画失败!");

			return;
		}

		aiAnimation* anim = scene->mAnimations[0];
		if (anim->mTicksPerSecond != 0.0f)
			m_ticksPerSecond = anim->mTicksPerSecond;
		else
			m_ticksPerSecond = 1;

		m_duration = anim->mDuration * anim->mTicksPerSecond;
		m_boneTransformsMap = {};

		for (int i = 0; i < anim->mNumChannels; i++) {
			aiNodeAnim* channel = anim->mChannels[i];
			BoneTransformTrack track;
			for (int j = 0; j < channel->mNumPositionKeys; j++) {
				track.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
				track.positions.push_back(assimpToGlmVec3(channel->mPositionKeys[j].mValue));
			}
			for (int j = 0; j < channel->mNumRotationKeys; j++) {
				track.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
				track.rotations.push_back(assimpToGlmQuat(channel->mRotationKeys[j].mValue));

			}
			for (int j = 0; j < channel->mNumScalingKeys; j++) {
				track.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
				track.scales.push_back(assimpToGlmVec3(channel->mScalingKeys[j].mValue));

			}
			m_boneTransformsMap[channel->mNodeName.C_Str()] = track;
		}
	}

	void Animation::OnAnimation(Ref<Model>& model, TimeStep ts)
	{
		m_timeContainer += ts;
		updateAnimation(model->GetBoneMap(), model->GetBoneMap()["mixamorig:Hips"], ts, glm::mat4(1.0f));
	}

	std::pair<uint32_t, float> Animation::getTimeFraction(std::vector<float>& times, float& dt) {
		uint32_t segment = 0;
		if (times.size() == 0)
		{
			return { 0, 0.0f };
		}
		while (dt > times[segment])
		{
			segment++;
			if (segment > times.size())
			{
				return { segment, 1.0f };
			}
		}
			
		float start = times[segment - 1];
		float end = times[segment];
		float frac = (dt - start) / (end - start);
		return { segment, frac };
	}

	void Animation::updateAnimation(std::unordered_map<std::string, Ref<Model::Bone>>& boneMap, Ref<Model::Bone>& bone, TimeStep ts, glm::mat4& parentTransform)
	{
		//根据骨骼获取当前骨骼的关键帧的Transform信息
		BoneTransformTrack& btt = m_boneTransformsMap[bone->name];
		//获取余数
		float dt = fmod(m_timeContainer, m_duration);
		std::pair<uint32_t, float> fp;
		//calculate interpolated position
		fp = getTimeFraction(btt.positionTimestamps, dt);

		glm::vec3 position1;
		glm::vec3 position2;

		if (fp.first > 0)
		{
			AR_ERROR("position{0}", fp.first);
			position1 = btt.positions[fp.first - 1];
			position2 = btt.positions[fp.first];
		}

		glm::vec3 position = glm::mix(position1, position2, fp.second);

		glm::quat rotation1;
		glm::quat rotation2;

		//calculate interpolated rotation
		fp = getTimeFraction(btt.rotationTimestamps, dt);
		
		if (fp.first > 0)
		{
			AR_ERROR("rotation{0}", fp.first);
			rotation1 = btt.rotations[fp.first - 1];
			rotation2 = btt.rotations[fp.first];
		}
		
			 
		glm::quat rotation = glm::mix(rotation1, rotation2, fp.second);

		glm::vec3 scale1;
		glm::vec3 scale2;

		//calculate interpolated scale
		fp = getTimeFraction(btt.scaleTimestamps, dt);
		
		if (fp.first > 0)
		{
			AR_ERROR("scale{0}", fp.first);
			glm::vec3 scale1 = btt.scales[fp.first - 1];
			glm::vec3 scale2 = btt.scales[fp.first];
		}

		glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

		glm::mat4 positionMat = glm::mat4(1.0),
			scaleMat = glm::mat4(1.0);

		rotation = glm::vec3(0.0f);
		//TODO::分析

		glm::mat4 rotationtemp = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

		glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), position)
			* rotationtemp
			* glm::scale(glm::mat4(1.0f), scale);
		if (localTransform == glm::mat4(0.0f))
		{
			localTransform = glm::mat4(1.0f);
		}
		glm::mat4 globalTransform = parentTransform * localTransform;

		boneMap[bone->name]->offset = globalTransform;
		//update values for children bones
		for (Ref<Model::Bone>& child : bone->chilren) {
			updateAnimation(boneMap, child, ts, boneMap[bone->name]->offset);
		}
		//std::cout << dt << " => " << position.x << ":" << position.y << ":" << position.z << ":" << std::endl;

	}
}