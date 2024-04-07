#pragma once

#include "Core.h"
#include "Adran/Events/Event.h"
#include "Window.h"
#include "Adran/Core/LayerStack.h"
#include "Adran/Events/ApplicationEvent.h"
#include "Adran/ImGui/ImGuiLayer.h"
#include "Adran/Renderer/RenderObject/Shader.h"
#include "Adran/Renderer/RenderObject/Buffer.h"
#include "Adran/Renderer/RenderObject/VertexArray.h"
#include "Adran/Renderer/Renderer.h"

#include "Adran/Core/TimeStep.h"

namespace Adran {
	class AR_API Application
	{
	public:
		Application(const std::string& windowTitle);
		virtual ~Application();

		void OnEvent(Event& e);

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();
		inline static Application& GetInstance(){ return *s_instance; };
		inline Window& GetWindow() { return *m_window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		//unique_ptr是智能指针，生命周期与普通变量相似
		Scope<Window> m_window;

		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
	private:
		static Application* s_instance;
	};

	Application* CreateApplication();
}

