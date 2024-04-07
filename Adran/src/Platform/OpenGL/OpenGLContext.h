#pragma once
#include "Adran/Renderer/GraphicsContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

namespace Adran
{
	class OpenGLContext :public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_windowHandle;
	};
}