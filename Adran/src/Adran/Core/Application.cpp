#include "AdranPreCompile.h"
#include "Application.h"
//此处貌似重复引入了"Event.h",但是Event.h中有#pragma once作用，所以不会报错
#include "Adran/Events/ApplicationEvent.h"
#include "Log.h"

#include "GLFW/glfw3.h"

#include "Adran/Core/Input.h"



namespace Adran {

	Application* Application::s_instance = nullptr;

	Application::Application(const std::string& windowTitle)
	{
		AR_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		//返回的指针转换为智能指针
		m_window = std::unique_ptr<Window>(Window::Create({windowTitle, 1440, 960}));
		m_window->SetEventCallback(AR_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);
		m_window->SetVSync(false);
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e) {

		EventDispatcher dispacher(e);

		dispacher.Dispatch<WindowCloseEvent>(AR_BIND_EVENT_FN(Application::OnWindowClose));
		dispacher.Dispatch<WindowResizeEvent>(AR_BIND_EVENT_FN(Application::OnWindowResize));


		for (Layer* layer : m_layerStack)
		{
			if (e.handled)
				break;
			layer->OnEvent(e);
		}

	}

	void Application::Run()
	{
		while (m_running) {

			float time = (float)glfwGetTime();
			TimeStep timeStep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized)
			{
				for (Layer* layer : m_layerStack)
					layer->OnUpdate(timeStep);
			}

			m_imGuiLayer->Begin();

			for (Layer* layer : m_layerStack)
				layer->OnImGuiRender();

			m_imGuiLayer->End();

			m_window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		AR_CORE_INFO("Adran is closed!");
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_minimized = true;
			return false;
		}
		m_minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
