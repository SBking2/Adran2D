#pragma once
#include "Adran/Core/Core.h"
#include "Adran/Core/TimeStep.h"

#include<glm/glm.hpp>
#include <vector>
#include <map>

namespace Adran
{
	class Animation
	{
	public:
		Animation(float duration, uint32_t samples);

		void AddPositionKeyFrame(uint32_t key, const glm::vec2& position);
		void AddRotationKeyFrame(uint32_t key, float rotation);
		void AddScaleKeyFrame(uint32_t key, const glm::vec2& scale);

		glm::vec2 GetPosition() { return m_position; }
		float GetRotation() { return m_rotation; }
		glm::vec2 GetScale() { return m_scale; }

		void OnUpdate(TimeStep ts);
	private:
		void UpdateKey(float timeContainer);
		void InterpolatePosition(float progress);
		void InterpolateRotation(float progress);
		void InterpolateScale(float progress);
		uint32_t FindNextKey(const std::map<uint32_t, glm::vec2>& keyMap, uint32_t currentKey);
		uint32_t FindNextKey(const std::map<uint32_t, float>& keyMap, uint32_t currentKey);
	private:
		std::map<uint32_t, glm::vec2> m_positionLineMap;
		std::map<uint32_t, float> m_rotationLineMap;
		std::map<uint32_t, glm::vec2> m_scaleLineMap;
	private:
		uint32_t m_lastPositionKey = 0;
		uint32_t m_nextPositionKey = 0;
		uint32_t m_lastRotationKey = 0;
		uint32_t m_nextRotationKey = 0;
		uint32_t m_lastScaleKey = 0;
		uint32_t m_nextScaleKey = 0;
	private:
		glm::vec2 m_position = { 0.0f, 0.0f };
		float m_rotation = 0.0f;
		glm::vec2 m_scale{ 0.0f, 0.0f };
	private:
		float m_gap;
		float m_timeContainer = 0.0f;
		float m_duration;
		uint32_t m_samples;
	};


}