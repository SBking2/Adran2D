#pragma once
#include<glm/glm.hpp>
#include "Camera.h"
namespace Adran 
{

	//SceneCamera是挂在在物体上的,因此不需要写Transform
	class SceneCamera : public Camera
	{
	public:
		enum class SceneCameraType {
			orthgraphic = 0,
			perspect = 1
		};

		SceneCamera(float aspectRatio = 0.0f);
		~SceneCamera();

		const glm::mat4& GetProjection() const;
		void SetViewPort(uint32_t width, uint32_t height);
		void ReCaculate();

		SceneCameraType& const GetSceneCameraType();
		void SetType(const SceneCameraType& type);

		void SetOtrh(float size, float nearClip, float farClip);
		void SetPer(float fov, float nearClip, float farClip);

		//Orthgraphic

		float const GetOrthNear();
		void SetOrthNear(float value);

		float const GetOrthFar();
		void SetOrthFar(float value);

		float const GetOrthSize();
		void SetOrthSize(float value);

		//Perspect
		float const GetPerNear();
		void SetPerNear(float value);

		float const GetPerFar();
		void SetPerFar(float value);

		float const GetPerFOV();
		void SetPerFOV(float value);

	private:
		float m_aspectRatio;
		SceneCameraType m_SceneCameraType = SceneCameraType::orthgraphic;

		//Perspect
		float m_SceneCameraFOV = glm::radians(45.0f);
		float m_perspectNear = 0.01f;
		float m_perspectFar = 1000.0f;

		//Orthgraphic
		float m_SceneCameraSize = 2.5f;
		float m_orthographicNear = -1.0f;
		float m_orthographicFar = 1.0f;
	};
}