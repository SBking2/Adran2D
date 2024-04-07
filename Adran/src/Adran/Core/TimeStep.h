#pragma once

namespace Adran {
	class TimeStep
	{
	public:
		TimeStep(float time):m_time(time)
		{

		}

		operator float() const { return m_time; }

		float GetSeconds() const { return m_time; }
		float GetMilliseconds() const { return m_time * 1000.0f; }
	private:
		float m_time;
	};
}