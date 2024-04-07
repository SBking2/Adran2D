#pragma once

#include "Adran/Renderer/Camera/OrthographicCamera.h"
#include "Adran/Core/TimeStep.h"

#include "Adran/Events/ApplicationEvent.h"
#include "Adran/Events/MouseEvent.h"

namespace Adran
{
	struct OrthographicCameraBounds
	{
		float left, right;
		float bottom, top;

		float GetWidth() { return right - left; }
		float GetHeight() { return top - bottom; }
	};
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = true);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& event);
		void OnResize(uint32_t width, uint32_t height);

		OrthographicCamera& GetCamera() { return m_camera; }
		const OrthographicCamera& GetCamera() const { return m_camera; }

		inline glm::vec3 GetPosition()const { return m_cameraPosition; }
		inline void SetPosition(const glm::vec3& position) { m_cameraPosition = position; }
		const OrthographicCameraBounds& GetBounds() { return m_bounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
	private:
		float m_aspectRatio;//¿í¸ß±È
		float m_zoomLevel = 1.0f;
		OrthographicCamera m_camera;
		OrthographicCameraBounds m_bounds;

		bool m_rotation;
		
		glm::vec3 m_cameraPosition;
		float m_cameraRotation;
		float m_cameraTranslationSpeed;
		float m_cameraRotationSpeed;
	};
}