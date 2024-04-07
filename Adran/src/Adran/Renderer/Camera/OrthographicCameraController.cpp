#include "AdranPreCompile.h"
#include "OrthographicCameraController.h"
#include "Adran/Core/KeyCodes.h"
#include "Adran/Core/Input.h"

namespace Adran
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_bounds({-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel}),
		m_aspectRatio(aspectRatio)
		,m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top)
		,m_rotation(rotation), m_cameraPosition(0.0f, 0.0f, 0.0f), m_cameraRotation(0.0f)
	{
		m_cameraRotationSpeed = 1.0f;
		m_cameraTranslationSpeed = 1.0f;
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		if (Adran::Input::IsKeyPressed(AR_KEY_LEFT))
		{
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;
		}

		if (Adran::Input::IsKeyPressed(AR_KEY_RIGHT))
		{
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;
		}

		if (Adran::Input::IsKeyPressed(AR_KEY_UP))
		{
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		}

		if (Adran::Input::IsKeyPressed(AR_KEY_DOWN))
		{
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;
		}

		if (m_rotation)
		{
			if (Adran::Input::IsKeyPressed(AR_KEY_A))
			{
				m_cameraRotation += m_cameraRotationSpeed * ts;
			}
			if (Adran::Input::IsKeyPressed(AR_KEY_D))
			{
				m_cameraRotation -= m_cameraRotationSpeed * ts;
			}

			m_camera.SetRotation(m_cameraRotation);
		}
		m_camera.SetPosition(m_cameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(AR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(uint32_t width, uint32_t height)
	{
		m_aspectRatio = width / (float) height;
		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
		m_camera.SetProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_zoomLevel -= event.GetYOffset() * 0.1f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		m_bounds = OrthographicCameraBounds({ -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel });
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_aspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		AR_TRACE(m_aspectRatio);
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		m_bounds = OrthographicCameraBounds({ -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel });
		return false;
	}

}