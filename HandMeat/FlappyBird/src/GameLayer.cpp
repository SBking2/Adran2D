#include "GameLayer.h"

#include "../vendor/imgui/imgui.h"

namespace FlappyBird
{

	GameLayer::GameLayer():Layer("GameLayer")
	{
		float windowWidth = Adran::Application::Get().GetWindow().GetWidth();
		float windowHeight = Adran::Application::Get().GetWindow().GetHeight();
		CreateCamera(windowWidth, windowHeight);
	}

	GameLayer::~GameLayer()
	{

	}

	void GameLayer::Init()
	{
		m_level.Init();
	}

	void GameLayer::OnAttach()
	{
		Init();
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(Adran::TimeStep ts)
	{
		if (m_running) {
			m_level.OnUpdate(ts);
			if (m_level.CollisionTest())
			{
				m_running = false;
				state = GameState::Over;
			}
		}

		m_camera->SetPosition(m_level.GetPlayer().position);

		//Input Check

		if (Adran::Input::IsMouseButtonPressed(AR_MOUSE_BUTTON_LEFT))
		{
			if (state == GameState::Over)
			{
				m_running = true;
				m_level.Reset();
			}
			if (state == GameState::Init)
			{
				m_running = true;
				state = GameState::Run;
			}
			state = GameState::Run;
		}

		//Before Render
		Adran::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		Adran::RenderCommand::Clear();

		//Render(先画的在上面)
		Adran::Renderer2D::BeginScene(*m_camera);

		m_level.OnRender();
		
		Adran::Renderer2D::EndScene();
	}

	void GameLayer::OnImGuiRender()
	{
		switch (state)
		{
		case FlappyBird::Init:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Adran::Application::Get().GetWindow().GetWidth();
			auto height = Adran::Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			ImGui::GetForegroundDrawList()->AddText(pos, 0xffffffff, "Click to Play!");
		}
			break;
		case FlappyBird::Run:
			break;
		case FlappyBird::Over:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Adran::Application::Get().GetWindow().GetWidth();
			auto height = Adran::Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			ImGui::GetForegroundDrawList()->AddText(pos, 0xffffffff, "Click to Restart!");
		}
			break;
		default:
			break;
		}
	}

	void GameLayer::OnEvent(Adran::Event& event)
	{
		Adran::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Adran::WindowResizeEvent>(AR_BIND_EVENT_FN(GameLayer::OnWindowResize));

	}

	bool GameLayer::OnWindowResize(Adran::WindowResizeEvent& event)
	{
		CreateCamera(event.GetWidth(), event.GetHeight());
		return false;
	}

	void GameLayer::CreateCamera(uint32_t width, uint32_t height)
	{
		float aspectRatio = (float)width / (float)height;

		float cameraWidth = 8.0f;
		float bottom = -cameraWidth;
		float top = cameraWidth;
		float left = bottom * aspectRatio;
		float right = top * aspectRatio;
		m_camera = Adran::CreateScope<Adran::OrthographicCamera>(left, right, bottom, top);
	}

}