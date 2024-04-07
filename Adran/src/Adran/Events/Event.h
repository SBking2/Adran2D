#pragma once

//This Header file contain these things
//1: EventType 事件类型
//2: EventCategory 事件属于哪个部分
//3: EVENT_CLASS_TYPE && EVENT_CLASS_CATEGOTY 宏的定义
//4：Event类的声明和定义
//5: EventDispatcher类
//6：运算符重写

#include "Adran/Core/Core.h"
#include "AdranPreCompile.h"

namespace Adran {
	//可能的事件类型，知名知其意
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		// Tick ？
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//EventCategory可能是用于辨识事件大体类型?
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication     = BIT(0),
		EventCategoryInput           = BIT(1),
		EventCategoryKeyboard        = BIT(2),
		EventCategoryMouse           = BIT(3),
		EventCategoryMouseButton     = BIT(4)
	};

//关于此处宏的知识点：##是连接, #是enum名字转为字符串,实在不知道为什么用virtual和override

//该宏返回事件类型的字符串？
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\
                               virtual EventType GetEventType() const override {return GetStaticType();}\
							   virtual const char* GetName() const override {return #type;}

//返回EventCategory类型的整型数
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	//事件基类
	class AR_API Event
	{

	public:
		//一些纯虚函数,若没有虚函数，则无法实现多态
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName();}

		//该函数是用来判断事件类型的,如果当前传进来的事件类和所要需求的category相同，则返回true
		inline bool IsInCategory(EventCategory category)
		{
			//&位与计算，因为categoty是根据位来区别，因此若不同，位与计算后是0
			return GetCategoryFlags() & category;
		}
		bool handled = false;
	};

	//？？？
	class EventDispatcher
	{
		template<typename T>//function函数实现Callback，EventFn<T>代表返回bool类型的形参为T&的函数
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) :m_Event(event)
		{

		}
		
		//该函数用于为Event辨别要执行的事件,指定的事件为func
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			//此处GetStaticType哪里来的？
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	//重写输出运算符，直接打印事件名字
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
