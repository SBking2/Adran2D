#pragma once
#include <Adran.h>

#include <glm/gtc/matrix_transform.hpp>

struct Particle
{
	Particle(const glm::vec3& position, float size, float rotation, const glm::vec4& color
		, const glm::vec3& velocity, float sizeVelocity, float rotationVelocity, float MaxLiveTime)
	{
		this->position = position;
		this->size = size;
		this->velocity = velocity;
		this->rotation = rotation;
		this->color = color;

		this->rotationVelocity = rotationVelocity;
		this->sizeVelocity = sizeVelocity;
		this->maxLiveTime = MaxLiveTime;
		this->liveTime = 0.0f;
	}

	void OnUpdate(Adran::TimeStep ts)
	{
		position += velocity * float(ts);
		rotation += rotationVelocity * float(ts);
		size += sizeVelocity * float(ts);
		liveTime += float(ts);
	}

	glm::vec3 position;
	float size;
	float rotation;
	float liveTime;
	glm::vec4 color;

	glm::vec3 velocity = glm::vec3(0.0f);
	float sizeVelocity = 0.0f;
	float rotationVelocity;
	float maxLiveTime;
};
class ParticleSystem
{
public:
	
	void Init(
		uint32_t maxNum, uint32_t minNum,
		float maxSize, float minSize,
		float maxRotation, float minRotation,
		float maxLiveTime, float minLiveTime,
		const glm::vec3& maxVelocity, const glm::vec3& minVelocity,
		float maxSizeVelocity, float minSizeVelocity,
		float maxRotationVelocity, float minRotationVelocity
	);
	void OnUpdate(Adran::TimeStep ts);
	void OnRender();
	void SetCreatePosition(const glm::vec3& position);
	void TakeMode(bool bl);

private:
	bool m_started = false;

	glm::vec3 m_createPosition = glm::vec3(0.0f);

	std::vector<Adran::Ref<Particle>> m_particles;

	uint32_t m_maxNum;
	uint32_t m_minNum;

	float m_maxSize = 0.0f;
	float m_minSize = 0.0f;

	float m_minRotation;
	float m_maxRotation;

	float m_maxLiveTime;
	float m_minLiveTime;

	glm::vec3 m_maxVelocity;
	glm::vec3 m_minVelocity;

	float m_maxSizeVelocity;
	float m_minSizeVelocity;

	float m_maxRotationVelocity;
	float m_minRotationVelocity;
};