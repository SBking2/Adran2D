#pragma once

#include<Adran.h>

namespace FlappyBird
{
	class Player
	{
	public:
		void Init();
		void OnUpdate(Adran::TimeStep ts);
		void OnRender();
	public:
		Adran::Ref<Adran::Texture2D> playerTex;
		glm::vec3 position;
		glm::vec2 speed;
		float rotation;
		float gravity = -9.8f;
		float jumpSpeed = 5.0f;

	};
}