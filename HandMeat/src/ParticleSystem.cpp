#include "ParticleSystem.h"

std::mt19937 Random::s_randomEngine;

std::uniform_int_distribution<std::mt19937::result_type> Random::s_distribution;

template<typename T>
static T GetRandom(const T& t1, const T& t2)
{
	T t = (t1 - t2) * Random::Float() + t2;
	return t;
}

void ParticleSystem::Init(
	uint32_t maxNum, uint32_t minNum,float maxSize, float minSize,float maxRotation, float minRotation,float maxLiveTime, float minLiveTime,
	const glm::vec3& maxVelocity, const glm::vec3& minVelocity,float maxSizeVelocity, float minSizeVelocity,
	float maxRotationVelocity, float minRotationVelocity
)
{
	this->m_maxNum = maxNum;
	this->m_minNum = minNum;

	this->m_maxSize = maxSize;
	this->m_minSize = minSize;

	this->m_maxRotation = maxRotation;
	this->m_minRotation = minRotation;

	this->m_maxLiveTime = maxLiveTime;
	this->m_minLiveTime = minLiveTime;

	this->m_maxVelocity = maxVelocity;
	this->m_minVelocity = minVelocity;

	this->m_maxSizeVelocity = maxSizeVelocity;
	this->m_minSizeVelocity = minSizeVelocity;

	this->m_maxRotationVelocity = maxRotationVelocity;
	this->m_minRotationVelocity = minRotationVelocity;
}

void ParticleSystem::OnUpdate(Adran::TimeStep ts)
{
	std::vector<Adran::Ref<Particle>>::iterator iter;
	for (iter = m_particles.begin(); iter != m_particles.end(); )
	{
		iter->get()->OnUpdate(ts);
		if (iter->get()->size <= 0.0f)
		{
			iter = m_particles.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	if (m_started)
	{
		uint32_t num = GetRandom(m_maxNum, m_minNum);
		num *= float(ts);
		for (uint32_t i = 0; i < num; i++)
		{
			float size = GetRandom(m_maxSize, m_minSize);
			float rotation = GetRandom(m_maxRotation, m_minRotation);
			float x = GetRandom(m_maxVelocity.x, m_minVelocity.x);
			float y = GetRandom(m_maxVelocity.y, m_minVelocity.y);
			float z = GetRandom(m_maxVelocity.z, m_minVelocity.z);
			glm::vec3 velocity = { x, y, z};
			float sizeVelocity = GetRandom(m_maxSizeVelocity, m_minSizeVelocity);
			float rotationVelocity = GetRandom(m_maxRotationVelocity, m_minRotationVelocity);
			float maxLiveTime = GetRandom(m_maxLiveTime, m_minLiveTime);
			glm::vec4 color = { Random::Float(), Random::Float(), Random::Float(), 1.0f };
			Adran::Ref<Particle> particle = Adran::CreateRef<Particle>(
				m_createPosition, size, rotation, color,
				velocity, sizeVelocity, rotationVelocity, maxLiveTime
				);
			m_particles.push_back(particle);
		}
	}
}

void ParticleSystem::OnRender()
{
	for (const auto& it : m_particles)
	{
		Adran::Renderer2D::DrawRotationQuad(it->position, glm::vec2(it->size), it->rotation,
			it->color);
	}
}

void ParticleSystem::SetCreatePosition(const glm::vec3& position)
{
	m_createPosition = position;
}

void ParticleSystem::TakeMode(bool bl)
{
	m_started = bl;
}