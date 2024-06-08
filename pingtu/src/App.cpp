#include <Adran.h>
#include "Adran/Core/EntryPoint.h"
#include "pingtuLayer.h"


class App :public Adran::Application
{
public:
	App(const std::string& windowTitle = "ping tu") :Application(windowTitle)
	{
		PushLayer(new Adran::pingtuLayer());
	}
	~App()
	{

	}
private:
};

Adran::Application* Adran::CreateApplication()
{
	return new App();
}