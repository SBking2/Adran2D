#include "AdranPreCompile.h"
#include "OpenGLContext.h"

namespace Adran
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	{
		m_windowHandle = windowHandle;
		AR_CORE_ASSERT(m_windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);

		//´´½¨OpenGLLoader
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AR_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		/*AR_CORE_INFO("OpenGL Info:");
		AR_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		AR_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		AR_CORE_INFO("Renderer: {0}", glGetString(GL_VERSION));*/

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}