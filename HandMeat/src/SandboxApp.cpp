#include <Adran.h>
#include "Adran/Core/EntryPoint.h"
#include "Sandbox2D.h"
#include "../FlappyBird/src/GameLayer.h"


class Sandbox :public Adran::Application
{
public:
	Sandbox(const std::string& windowTitle = "Sandbox App") :Application(windowTitle)
	{
		PushLayer(new Sandbox2DLayer());
	}
	~Sandbox()
	{

	}
private:
};

Adran::Application* Adran::CreateApplication()
{
	return new Sandbox();
}