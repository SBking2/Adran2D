#pragma once

#include "Adran/Core/Core.h"
#include "Adran/Events/Event.h"
#include "Adran/Core/TimeStep.h"
namespace Adran {
	class AR_API Layer
	{
	public:
		Layer(const std::string& name = "Default Layer");
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(TimeStep ts);
		virtual void OnImGuiRender();
		virtual void OnEvent(Event& event);

		inline const std::string& GetName() const { return m_name; }
	protected:
		std::string m_name;
	};

}

