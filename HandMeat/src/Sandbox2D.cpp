#include "Sandbox2D.h"
#include "../vendor/imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>

Sandbox2DLayer::Sandbox2DLayer():Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f)
{
}

void Sandbox2DLayer::OnAttach()
{
	m_particleSystem.Init(
		500, 200,
		0.3f, 0.1f,
		180.0f, 0.0f,
		0.1f, 0.06f,
		{ 0.2f, 0.2f, 0.0f }, {-0.2f, -0.2f, 0.0f},
		-1.0f, -0.1f,
		1.0f, -1.0f
	);
	m_AmiyaTex = Adran::Texture2D::Create("assets/textures/SBKING.bmp");
	m_RPGBaseTex = Adran::Texture2D::Create("assets/textures/RPGBase.png");
	uint32_t* stairs = new uint32_t[4];
	stairs[0] = 8;
	stairs[1] = 8;
	stairs[2] = 9;
	stairs[3] = 9;
	m_solidSubTex = Adran::SubTexture::CreateSubTexture(m_RPGBaseTex, { 128.0f, 128.0f }, { 2560.0f, 1664.0f }, stairs);
    Adran::FrameBufferSpecification spec;
    spec.width = 1920;
    spec.height = 1080;
}

void Sandbox2DLayer::OnDetach()
{

}

void Sandbox2DLayer::OnUpdate(Adran::TimeStep ts)
{
	if (Adran::Input::IsMouseButtonPressed(AR_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Adran::Input::GetMousePosition();
		auto width = Adran::Application::GetInstance().GetWindow().GetWidth();
		auto height = Adran::Application::GetInstance().GetWindow().GetHeight();

		auto bounds = m_cameraController.GetBounds();
		auto pos = m_cameraController.GetCamera().GetPosition();

		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_particleSystem.SetCreatePosition({ x + pos.x, y + pos.y, 0.0f });
		m_particleSystem.TakeMode(true);
	}
	else
	{
		m_particleSystem.TakeMode(false);
	}

	m_cameraController.OnUpdate(ts);
	m_particleSystem.OnUpdate(ts);

	Adran::Renderer2D::s_statics.timeStep = ts.GetMilliseconds();
	Adran::Renderer2D::s_statics.fps = 1000.0f / ts.GetMilliseconds();


	Adran::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
	Adran::RenderCommand::Clear();
	

	Adran::Renderer2D::BeginScene(m_cameraController.GetCamera());
	/*for (uint32_t i = 0; i < 10; i++)
	{
		for (uint32_t j = 0; j < 10; j++)
		{
			Adran::Renderer2D::DrawQuad({ -2.0f + i*0.5f,  -2.0f+j*0.5f }, { 0.45f, 0.45f }, { i/10.0f, j/10.0f, 0.0f, 1.0f });
		}
	}*/
	
	m_particleSystem.OnRender();
	Adran::Renderer2D::DrawQuad({ 0.0f,  1.0f }, { 1.0f, 1.0f }, m_AmiyaTex, glm::vec4(1.0f));
	//Adran::Renderer2D::DrawQuad({ 0.0f,  0.0f }, { 1.0f, 1.0f }, m_solidSubTex, glm::vec4(1.0f));
	//Adran::Renderer2D::DrawQuad({ 0.0f,  0.0f }, { 10.0f, 10.0f }, glm::vec4(1.0f));
	Adran::Renderer2D::EndScene();
}

void Sandbox2DLayer::OnEvent(Adran::Event& event)
{
	m_cameraController.OnEvent(event);
	Adran::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Adran::KeyPressedEvent>(AR_BIND_EVENT_FN(Sandbox2DLayer::OnKeyPressedEvent));
}

bool Sandbox2DLayer::OnKeyPressedEvent(Adran::KeyPressedEvent& event)
{
	return false;
}

void Sandbox2DLayer::OnImGuiRender()
{

    ImGui::Begin("Settings");
    ImGui::Text("FPS: %d", Adran::Renderer2D::s_statics.fps);
    ImGui::Text("TimeStep:%f", Adran::Renderer2D::s_statics.timeStep);
    ImGui::Text("DrawCall:%d", Adran::Renderer2D::s_statics.drawCallTime);
    ImGui::End();

}
