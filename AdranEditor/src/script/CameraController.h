#pragma once
#include <Adran.h>

using namespace Adran;

class CameraController : public Adran::Script
{
public:
	virtual void OnCreate()override 
	{

	}

	virtual void OnUpdate(TimeStep ts) override
	{
		auto& transform = GetComponent<TransformComponent>();
		auto& cc = GetComponent<CameraComponent>();
		float speed = 1.0f;

		if (Input::IsKeyPressed(KeyCode::AR_A))
			transform.position.x -= speed * ts;
		if(Input::IsKeyPressed(KeyCode::AR_D))
			transform.position.x += speed * ts;
		if(Input::IsKeyPressed(KeyCode::AR_W))
			transform.position.y += speed * ts;
		if(Input::IsKeyPressed(KeyCode::AR_S))
			transform.position.y -= speed * ts;

		if (Input::IsKeyPressed(AR_UP))
		{
			float size = cc.camera.GetOrthSize();
			cc.camera.SetOrthSize(size -= ts * speed);
		}
		if (Input::IsKeyPressed(AR_DOWN))
		{
			float size = cc.camera.GetOrthSize();
			cc.camera.SetOrthSize(size += ts * speed);
		}
	}

	virtual void OnDestory() override
	{

	}
};