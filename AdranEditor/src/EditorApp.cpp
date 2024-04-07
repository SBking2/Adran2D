#include <Adran.h>
#include "Adran/Core/EntryPoint.h"
#include "EditorLayer.h"


class EditorApp :public Adran::Application
{
public:
	EditorApp(const std::string& windowTitle = "Adran Editor") :Application(windowTitle)
	{
		PushLayer(new Adran::EditorLayer());
	}
	~EditorApp()
	{

	}
private:
};

Adran::Application* Adran::CreateApplication()
{
	return new EditorApp();
}