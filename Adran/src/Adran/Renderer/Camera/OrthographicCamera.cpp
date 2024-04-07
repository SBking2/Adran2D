#include "AdranPreCompile.h"
#include "OrthographicCamera.h"

#include<glm/gtc/matrix_transform.hpp>

namespace Adran
{

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
	{
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
		m_position = glm::vec3(0.0f);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * 
			glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

}