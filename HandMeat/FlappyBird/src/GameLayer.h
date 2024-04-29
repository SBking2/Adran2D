#pragma once

#include "Level.h"

namespace FlappyBird
{
	enum GameState
	{
		Init,
		Run,
		Over
	};
	class GameLayer : public Adran::Layer
	{
	public:
		GameLayer();
		virtual ~GameLayer();

		virtual void Init();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Adran::TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Adran::Event& event) override;
	private:
		bool OnWindowResize(Adran::WindowResizeEvent& event);
		void CreateCamera(uint32_t width, uint32_t height);
	private:
		Level m_level;
		Adran::Scope<Adran::OrthographicCamera> m_camera;
		bool m_running = false;
		GameState state = GameState::Init;
	};
}
