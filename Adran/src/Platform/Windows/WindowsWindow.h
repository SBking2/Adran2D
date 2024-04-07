#pragma once
//WindowsWindow类封装了GLFW的window相关方法
#include "Adran/Core/Window.h"
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Adran/Renderer/GraphicsContext.h"

namespace Adran {
	class WindowsWindow:public Window
	{
	public:
		WindowsWindow(const WindowInfo& infos);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_winData.width; }
		inline unsigned int GetHeight() const override { return m_winData.height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			m_winData.eventCallback = callback;
		}
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_window; }
	private:
		virtual void Init(const WindowInfo& infos);
		virtual void Shutdown();
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSyns;

			EventCallbackFn eventCallback;
		};

		WindowData m_winData;
	};
}

