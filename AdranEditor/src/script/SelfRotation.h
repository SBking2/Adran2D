#pragma once
#include <Adran.h>

using namespace Adran;

class SelfRotation : public Adran::Script
{
public:
	virtual void OnCreate()override
	{
		float x = Random::Float() * 5 + -2.5;
		float y = Random::Float() * 5 + -2.5;
		auto& transform = GetComponent<TransformComponent>();
		transform.position.x = x;
		transform.position.y = y;
	}

	virtual void OnUpdate(TimeStep ts) override
	{
		auto& transform = GetComponent<TransformComponent>();
		transform.rotation.z += speed * ts;
	}

	virtual void OnDestory() override
	{

	}
private:
	float speed = 1.0f;
};