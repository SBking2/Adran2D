#include "AdranPreCompile.h"
#include "EditorCamera.h"

#include "Adran/Core/Input.h"
#include "Adran/Core/KeyCodes.h"
#include "Adran/Core/MouseCodes.h"

#include <glfw/glfw3.h>

#include <glm/gtx/quaternion.hpp>

namespace Adran {
	EditorCamera::EditorCamera(float aspectRatio)
		:m_aspectRatio(aspectRatio), m_projectionMatrix(glm::ortho(-m_aspectRatio * m_zoomlevel, m_aspectRatio* m_zoomlevel, -m_zoomlevel, m_zoomlevel, -1.0f, 1.0f))
		, m_viewMatrix(1.0f)
	{
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
		m_position = glm::vec3(0.0f);
	}

	void EditorCamera::SetViewport(uint32_t width, uint32_t height)
	{
		m_aspectRatio = (float)width / (float)height;
		m_projectionMatrix = glm::ortho(-m_aspectRatio * m_zoomlevel, m_aspectRatio * m_zoomlevel, -m_zoomlevel, m_zoomlevel, -1.0f, 1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void EditorCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void EditorCamera::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(KeyCode::AR_LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_initMousePosition) * 0.003f;
			m_initMousePosition = mouse;

			if (Input::IsMouseButtonPressed(MouseCode::MOUSE_BUTTON_LEFT))
			{
				m_position.x +=- delta.x;
				m_position.y += delta.y;
			}
		}

		RecalculateViewMatrix();

	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>(AR_BIND_EVENT_FN(EditorCamera::MouseScroller));

	}

	bool EditorCamera::MouseScroller(MouseScrolledEvent& event)
	{
		m_zoomlevel -= event.GetYOffset() * m_zoomSpeed;
		if (m_zoomlevel < 0.5f)
			m_zoomlevel = 0.5f;

		return true;
	}


}

