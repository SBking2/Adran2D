#include "AdranPreCompile.h"
#include "SceneCamera.h"

#include<glm/gtc/matrix_transform.hpp>

namespace Adran
{
	SceneCamera::SceneCamera(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		ReCaculate();
	}

	SceneCamera::~SceneCamera()
	{

	}

	const glm::mat4& SceneCamera::GetProjection ()const
	{
		return m_projection;
	}

	void SceneCamera::SetViewPort(uint32_t width, uint32_t height)
	{
		m_aspectRatio = (float)width / (float)height;
		ReCaculate();
	}
	void SceneCamera::ReCaculate()
	{
		if (m_SceneCameraType == SceneCameraType::orthgraphic)
		{
			float left = m_aspectRatio * m_orthographicNear * m_SceneCameraSize;
			float right = m_aspectRatio * m_orthographicFar * m_SceneCameraSize;
			float bottom = m_orthographicNear * m_SceneCameraSize;
			float top = m_orthographicFar * m_SceneCameraSize;
			m_projection = glm::ortho(left, right, bottom, top, m_orthographicNear, m_orthographicFar);
		}
		else {
			m_projection = glm::perspective(m_SceneCameraFOV, m_aspectRatio, m_perspectNear, m_perspectFar);
		}
	}
	SceneCamera::SceneCameraType& const SceneCamera::GetSceneCameraType()
	{
		return m_SceneCameraType;
	}
	void SceneCamera::SetType(const SceneCameraType& type)
	{
		m_SceneCameraType = type;
		ReCaculate();
	}

	void SceneCamera::SetOtrh(float size, float nearClip, float farClip)
	{
		m_SceneCameraSize = size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;
		ReCaculate();
	}
	void SceneCamera::SetPer(float fov, float nearClip, float farClip)
	{
		m_SceneCameraFOV = fov;
		m_perspectNear = nearClip;
		m_perspectFar = farClip;
		ReCaculate();
	}

	//Orthgraphic
	float const SceneCamera::GetOrthNear() { return m_orthographicNear; }
	void SceneCamera::SetOrthNear(float value) { m_orthographicNear = value; ReCaculate();}

	float const SceneCamera::GetOrthFar() { return m_orthographicFar; }
	void SceneCamera::SetOrthFar(float value) { m_orthographicFar = value; ReCaculate();}

	float const SceneCamera::GetOrthSize() { return m_SceneCameraSize; }
	void SceneCamera::SetOrthSize(float value) { m_SceneCameraSize = value; ReCaculate(); }

	//Perspect
	float const SceneCamera::GetPerNear() { return m_perspectNear; }
	void SceneCamera::SetPerNear(float value) { m_perspectNear = value; ReCaculate();}

	float const SceneCamera::GetPerFar() { return m_perspectFar; }
	void SceneCamera::SetPerFar(float value) { m_perspectFar = value; ReCaculate();}

	float const SceneCamera::GetPerFOV() { return m_SceneCameraFOV; }
	void SceneCamera::SetPerFOV(float value) { m_SceneCameraFOV = value; ReCaculate();}
}
