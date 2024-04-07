#include "Player.h"

#include <cmath>
namespace FlappyBird
{
	// Linear Interpolation
	double lerp(double start, double end, double t) {
		return start + t * (end - start);
	}

	void Player::Init()
	{
		playerTex = Adran::Texture2D::Create("FlappyBird/assets/tex/player.png");
		position = glm::vec3(0.0f);
		speed = {3.0f, 0.0f};
		rotation = 0.0f;
		gravity = -9.8f;
		jumpSpeed = 5.0f;
	}

	void Player::OnUpdate(Adran::TimeStep ts)
	{

		speed.y += gravity * ts;

		if (Adran::Input::IsKeyPressed(AR_KEY_SPACE))
		{
			speed.y = jumpSpeed;
		}

		position.x += speed.x * ts;
		position.y += speed.y * ts;

		rotation = lerp(rotation, std::atan(speed.y) * 180.0 / 3.1415926535f, 3 * ts);
	}

	void Player::OnRender()
	{
		Adran::Renderer2D::DrawRotationQuad(position, { 3.0f, 3.0f }, glm::radians(rotation), playerTex, glm::vec4(1.0f));
	}

}