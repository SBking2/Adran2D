#include "AdranPreCompile.h"
#include "WindowsWindow.h"

#include "Adran/Events/ApplicationEvent.h"
#include "Adran/Events/KeyEvent.h"
#include "Adran/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Adran {
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		AR_CORE_ERROR("GLFW Error ({0}):{1}", error, description);
	}

	Scope<Window> Window::Create(const WindowInfo& infos)
	{
		return CreateScope<WindowsWindow>(infos);
	}

	WindowsWindow::WindowsWindow(const WindowInfo& infos)
	{
		Init(infos);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_winData.VSyns = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_winData.VSyns;
	}

	void WindowsWindow::Init(const WindowInfo& infos)
	{

		m_winData.title = infos.title;
		m_winData.width = infos.width;
		m_winData.height = infos.height;

		AR_CORE_INFO("Creating window {0} ({1},{2})", m_winData.title, m_winData.width, m_winData.height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			AR_CORE_ASSERT(success, "Could not initialize GLFW!");
			//callback对接
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)m_winData.width, (int)m_winData.height, m_winData.title.c_str(), nullptr, nullptr);
		
		m_context = new OpenGLContext(m_window);
		m_context->Init();

		glfwSetWindowUserPointer(m_window, &m_winData);
		SetVSync(true);
		
		#pragma region Callback Setting glew的事件用Event System包裹

		//Callback,意思是进行在某步时调用该函数,此处是m_window,resize的时候调用lamda函数
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;

				WindowResizeEvent resizeEvent(data.width, data.height);
				data.eventCallback(resizeEvent);
			}
		);
		
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent closeEvent;
				data.eventCallback(closeEvent);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
				}
			});
		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int character)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(character);
				data.eventCallback(event);
			});
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent  event((float)xOffset, (float)yOffset);
				data.eventCallback(event);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.eventCallback(event);
			});
	#pragma endregion
		
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

}