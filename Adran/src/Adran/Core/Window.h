#pragma once

#include "AdranPreCompile.h"

#include "Adran/Core/Core.h"
#include "Adran/Events/Event.h"

namespace Adran {
	 //窗口信息
	 struct WindowInfo
	 {
		 std::string title;
		 unsigned int width;
		 unsigned int height;

		 WindowInfo(const std::string& title = "Adran Engine", unsigned int width = 1280, unsigned int height = 720)
		 {
			 this->title = title;
			 this->width = width;
			 this->height = height;
		 }
	 };

	 //窗口基类，Adran写Window，派生类是不同平台的不同定义
	 class AR_API Window
	 {
	 public:
		 //类型别名
		 using EventCallbackFn = std::function<void(Event&)>;

		 virtual ~Window(){}
		 virtual void OnUpdate() = 0;
		 virtual unsigned int GetWidth() const = 0;
		 virtual unsigned int GetHeight() const = 0;
		 
		 //VSync是垂直同步
		 virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		 virtual void SetVSync(bool enabled) = 0;
		 virtual bool IsVSync() const = 0;

		 virtual void* GetNativeWindow() const = 0;

		 static Scope<Window> Create(const WindowInfo& infos = WindowInfo());
	 };
}