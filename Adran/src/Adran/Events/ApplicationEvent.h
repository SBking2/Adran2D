#pragma once

#include "Event.h"

namespace Adran {
	
	//---------------------Resize Event-------------------------------
	class AR_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			:m_width(width),m_height(height){}

		inline unsigned int GetWidth() const { return m_width; }
		inline unsigned int GetHeight() const { return m_height; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "WindowResize:" << m_width << ", " << m_height;
			return ss.str();
		}
		
		//巨牛逼，用宏来辨别类型
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_width, m_height;
	};
	//-----------------------------------------------------------
	//---------------------Win Close Event-----------------------
	class AR_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(){}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	//-----------------------------------------------------------
	//---------------------App Tick Event-----------------------
	class AR_API AppTickEvent : public Event
	{
	public:
		AppTickEvent(){}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	//-----------------------------------------------------------
	//---------------------App Update Event-----------------------
	class AR_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	//-----------------------------------------------------------
	//---------------------App Render Event-----------------------
	class AR_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	//-----------------------------------------------------------
}