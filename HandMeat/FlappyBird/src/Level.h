#pragma once

#include <Adran.h>
#include "Player.h"

namespace FlappyBird
{
	

	struct Barrier
	{
		glm::vec3 topPosition;
		glm::vec3 bottomPosition;
		float topScale;
		float bottomScale;
	};
	class Level
	{
	public:
		void Init();
		void Reset();
		void OnUpdate(Adran::TimeStep ts);
		void OnRender();
		bool CollisionTest();
		inline Player GetPlayer()const { return m_player; }
	private:
		float crossProduct(const glm::vec2& v1, const glm::vec2& v2);
		bool PointInTri(const glm::vec2& position, const glm::vec2& A, const glm::vec2& B, const glm::vec2& C);
		void CreateBarrier(int index, float offset);

	private:
		Player m_player;
		std::vector<Barrier> m_barriers;
		Adran::Ref<Adran::Texture2D> m_trangleTex;

		int barrierIndex = 0;

		float heightHalf = 12.0f;
		float GapMax = 5.0f;
	};
}