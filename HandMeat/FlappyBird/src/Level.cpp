#include "Level.h"
#include "Random.h"

#include <glm/gtc/matrix_transform.hpp>
using namespace RandomR;

namespace FlappyBird
{

	void Level::Init()
	{
		Random::Init();
		m_player.Init();
		m_trangleTex = Adran::Texture2D::Create("FlappyBird/assets/tex/barrier.png");
		m_barriers.resize(5);
		for (int index = 0; index < 5; index++)
		{
			CreateBarrier(index, (index + 1) * 15.0f);
		}
	}

	void Level::Reset()
	{
		m_player.position = glm::vec4(0.0f);
		m_player.rotation = glm::radians(0.0f);
		m_player.speed = { 3.0f, 0.0f };
		for (int index = 0; index < 5; index++)
		{
			CreateBarrier(index, (index + 1) * 15.0f);
		}
	}

	void Level::OnUpdate(Adran::TimeStep ts)
	{
		m_player.OnUpdate(ts);

		if (m_barriers[barrierIndex].bottomPosition.x < m_player.position.x - 22.5f)
		{
			CreateBarrier(barrierIndex, m_barriers[(barrierIndex - 1 + m_barriers.size()) % m_barriers.size()].topPosition.x + 20.0f);
			barrierIndex = (barrierIndex + 1) % m_barriers.size();
		}
	}

	void Level::OnRender()
	{
		m_player.OnRender();
		for (const auto& barrier : m_barriers)
		{
			Adran::Renderer2D::DrawRotationQuad(barrier.topPosition, glm::vec2(barrier.topScale), glm::radians(180.0f), m_trangleTex, glm::vec4(1.0f), 1.0f);
			Adran::Renderer2D::DrawRotationQuad(barrier.bottomPosition, glm::vec2(barrier.bottomScale), 0.0f, m_trangleTex, glm::vec4(1.0f), 1.0f);
		}
		Adran::Renderer2D::DrawQuad({ m_barriers[barrierIndex].bottomPosition.x, heightHalf + 5.0f, 0.0f }, { 100.0f, 10.0f }, glm::vec4(1.0f));
		Adran::Renderer2D::DrawQuad({ m_barriers[barrierIndex].bottomPosition.x, -heightHalf - 5.0f, 0.0f }, { 100.0f, 10.0f }, glm::vec4(1.0f));
	}



	float Level::crossProduct(const glm::vec2& v1, const glm::vec2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	bool Level::PointInTri(const glm::vec2& position, const glm::vec2& A, const glm::vec2& B, const glm::vec2& C)
	{
		glm::vec2 AB(B.x - A.x, B.y - A.y);
		glm::vec2 BC(C.x - B.x, C.y - B.y);
		glm::vec2 CA(A.x - C.x, A.y - C.y);

		glm::vec2 AP(position.x - A.x, position.y - A.y);
		glm::vec2 BP(position.x - B.x, position.y - B.y);
		glm::vec2 CP(position.x - C.x, position.y - C.y);

		// 计算叉积
		float crossAB = crossProduct(AB, AP);
		float crossBC = crossProduct(BC, BP);
		float crossCA = crossProduct(CA, CP);

		// 判断点是否在三角形内
		return (crossAB >= 0 && crossBC >= 0 && crossCA >= 0) || (crossAB <= 0 && crossBC <= 0 && crossCA <= 0);
	}

	void Level::CreateBarrier(int index, float offset)
	{

		Barrier& barrier = m_barriers[index];
		barrier.topPosition.x = offset;
		barrier.bottomPosition.x = offset;
		barrier.topPosition.z = index * 0.1f - 0.5f;
		barrier.bottomPosition.z = index * 0.1f - 0.5f + 0.05;

		float center = Random::Float() * heightHalf - heightHalf/2;
		float gap = 3.0f + Random::Float() * GapMax;

		float topDing = center + gap / 2;
		float bottomDing = center - gap / 2;
		barrier.topPosition.y = (heightHalf + topDing)/2 ;
		barrier.bottomPosition.y = (-heightHalf + bottomDing) / 2;

		barrier.topScale = heightHalf - topDing;
		barrier.bottomScale = bottomDing - (-heightHalf);
	}

	bool Level::CollisionTest()
	{
		if (glm::abs(m_player.position.y) > 11.0f)
			return true;

		float playerSize = 3.0f;
		glm::vec4 playervertices[4] =
		{
			{-0.3f * playerSize, -0.3f * playerSize, 0.0f, 1.0f},
			{ 0.3f * playerSize, -0.3f * playerSize, 0.0f, 1.0f},
			{ 0.3f * playerSize,  0.3f * playerSize, 0.0f, 1.0f},
			{-0.3f * playerSize,  0.3f * playerSize, 0.0f, 1.0f}
		};

		const auto& position = m_player.position;
		glm::vec4 playerTransfromVertices[4];
		for (int i = 0; i < 4; i++)
		{
			playerTransfromVertices[i] = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_player.rotation), { 0.0f,0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f })
				* playervertices[i];
		}

		glm::vec4 barrierVertices[3] =
		{
			{-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
			{ 0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
			{ 0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f}
		};

		for (auto& b : m_barriers)
		{
			glm::vec2 tri[3];

			for (int i = 0; i < 3; i++)
			{
				tri[i] = glm::translate(glm::mat4(1.0f), { b.topPosition.x, b.topPosition.y, 0.0f })
					* glm::rotate(glm::mat4(1.0), glm::radians(180.0f), { 0.0f, 0.0f, 1.0f })
					* glm::scale(glm::mat4(1.0f), { b.topScale , b.topScale, 1.0f })
					* barrierVertices[i];
			}

			for (auto& vert : playerTransfromVertices)
			{
				if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
					return true;
			}

			for (int i = 0; i < 3; i++)
			{
				tri[i] = glm::translate(glm::mat4(1.0f), { b.bottomPosition.x, b.bottomPosition.y, 0.0f })
					* glm::scale(glm::mat4(1.0f), { b.bottomScale , b.bottomScale, 1.0f })
					* barrierVertices[i];
			}

			for (auto& vert : playerTransfromVertices)
			{
				if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
					return true;
			}
		}

		return false;
	}

}