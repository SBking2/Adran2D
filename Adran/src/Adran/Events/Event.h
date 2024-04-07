#pragma once

//This Header file contain these things
//1: EventType �¼�����
//2: EventCategory �¼������ĸ�����
//3: EVENT_CLASS_TYPE && EVENT_CLASS_CATEGOTY ��Ķ���
//4��Event��������Ͷ���
//5: EventDispatcher��
//6���������д

#include "Adran/Core/Core.h"
#include "AdranPreCompile.h"

namespace Adran {
	//���ܵ��¼����ͣ�֪��֪����
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		// Tick ��
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//EventCategory���������ڱ�ʶ�¼���������?
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication     = BIT(0),
		EventCategoryInput           = BIT(1),
		EventCategoryKeyboard        = BIT(2),
		EventCategoryMouse           = BIT(3),
		EventCategoryMouseButton     = BIT(4)
	};

//���ڴ˴����֪ʶ�㣺##������, #��enum����תΪ�ַ���,ʵ�ڲ�֪��Ϊʲô��virtual��override

//�ú귵���¼����͵��ַ�����
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\
                               virtual EventType GetEventType() const override {return GetStaticType();}\
							   virtual const char* GetName() const override {return #type;}

//����EventCategory���͵�������
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	//�¼�����
	class AR_API Event
	{

	public:
		//һЩ���麯��,��û���麯�������޷�ʵ�ֶ�̬
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName();}

		//�ú����������ж��¼����͵�,�����ǰ���������¼������Ҫ�����category��ͬ���򷵻�true
		inline bool IsInCategory(EventCategory category)
		{
			//&λ����㣬��Ϊcategoty�Ǹ���λ�������������ͬ��λ��������0
			return GetCategoryFlags() & category;
		}
		bool handled = false;
	};

	//������
	class EventDispatcher
	{
		template<typename T>//function����ʵ��Callback��EventFn<T>������bool���͵��β�ΪT&�ĺ���
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) :m_Event(event)
		{

		}
		
		//�ú�������ΪEvent���Ҫִ�е��¼�,ָ�����¼�Ϊfunc
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			//�˴�GetStaticType�������ģ�
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

	//��д����������ֱ�Ӵ�ӡ�¼�����
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
