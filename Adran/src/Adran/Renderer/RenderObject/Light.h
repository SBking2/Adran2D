#pragma once

#include "Shader.h"
#include <glm/glm.hpp>
namespace Adran
{
	class Light
	{
	public:
		Light(const glm::vec3& position = glm::vec3(0.0f), float strength = 1.0f);
		inline const glm::vec3& GetPosition() { return m_position; }
		inline float GetStrength() { return m_strength; }

		void Bind(const Ref<Shader>& shader);

		inline void SetPosition(const glm::vec3& position)
		{
			m_position = position;
		}
		inline void SetStrength(float value)
		{
			m_strength = value;
		}
	private:
		float m_strength;
		glm::vec3 m_position;
	};
}