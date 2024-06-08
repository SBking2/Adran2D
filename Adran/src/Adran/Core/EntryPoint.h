#pragma once

#ifdef AR_PLATFORM_WINDOWS

extern Adran::Application* Adran::CreateApplication();

int main(int argc, char** argv)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	Adran::Log::Init();
	AR_INFO("The client info!");

	auto app = Adran::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#endif