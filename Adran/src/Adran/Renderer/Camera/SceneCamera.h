#pragma once
#include<glm/glm.hpp>
#include "Camera.h"
namespace Adran 
{

	//SceneCamera是挂在在物体上的,因此不需要写Transform
	class SceneCamera : public Camera
	{
	public:

		SceneCamera(float aspectRatio = 0.0f);
		~SceneCamera();

		const glm::mat4& GetProjection() const;
		void SetViewPort(uint32_t width, uint32_t height);
		void ReCaculate();

		void SetOtrh(float size, float nearClip, float farClip);

		//Orthgraphic

		float const GetOrthNear();
		void SetOrthNear(float value);

		float const GetOrthFar();
		void SetOrthFar(float value);

		float const GetOrthSize();
		void SetOrthSize(float value);

	private:
		float m_aspectRatio;

		//Orthgraphic
		float m_SceneCameraSize = 2.5f;
		float m_orthographicNear = -1.0f;
		float m_orthographicFar = 1.0f;
	};
}