#include "AdranPreCompile.h"
#include "ImGuiLayer.h"
#include "imgui.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "ImGuizmo.h"

#include "Adran/Core/Application.h"
namespace Adran {
	Adran::ImGuiLayer::ImGuiLayer():Layer("ImGuiLayer")
	{

	}

	Adran::ImGuiLayer::~ImGuiLayer()
	{

	}

	//What have he fucking done?
	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //enabled keyboard controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ ViewportsNoMerge;

		//设置字体（C++相对路径是指当前程序运行的路径）
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets\\fronts\\Titillium_Web\\TitilliumWeb-Regular.ttf", 20.0f);
		SetThemColor();

		//ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End() {

		ImGuiIO& io = ImGui::GetIO();

		Application& app = Application::GetInstance();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//Rendering

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		//ImGui输入或者glfw都会让该layer置为focused
		e.handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		e.handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	void ImGuiLayer::SetThemColor()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f,0.305f, 0.31f, 1.0f }; 
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f,0.205f, 0.21f,1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f,1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f,0.3805f,0.381f,1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f,0.281f,1.0f};
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f,0.205f,0.21f,1.0f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f,0.1505f,0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f,0.1505f,0.951f,1.0f };

	}
}

