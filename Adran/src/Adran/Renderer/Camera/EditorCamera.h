#pragma once

#include "Camera.h"
#include "Adran/Core/Timestep.h"
#include "Adran/Events/Event.h"
#include "Adran/Events/MouseEvent.h"


#include <glm/glm.hpp>
namespace Adran
{
	class EditorCamera
	{
	public:
		EditorCamera(float aspectRatio);

		inline const glm::vec3& GetPosition()const { return m_position; }
		inline void SetPosition(const glm::vec3& position)
		{
			m_position = position;
			RecalculateViewMatrix();
		}

		void SetViewport(uint32_t width, uint32_t height);

		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

		//以上是从OrthoCamera Copy过来的，一下为操作逻辑
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& event);
	private:
		void RecalculateViewMatrix();
		bool MouseScroller(MouseScrolledEvent& event);
	private:

		float m_zoomSpeed = 0.1f;
		float m_moveSpeed = 1.0f;

		float m_zoomlevel = 1.0f;
		float m_aspectRatio = 0.0f;

		glm::vec2 m_initMousePosition = { 0.0f, 0.0f };
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position;
	};
}