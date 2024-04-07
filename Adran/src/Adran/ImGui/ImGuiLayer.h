#pragma once
#include "Adran/Core/Layer.h"

#include "Adran/Events/MouseEvent.h"
#include "Adran/Events/ApplicationEvent.h"
#include "Adran/Events/KeyEvent.h"
namespace Adran {
	class AR_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(TimeStep ts) override {};
		virtual void OnEvent(Event& e)override;
		void SetThemColor();

		void Begin();
		void End();
	private:
		float m_time = 0.0f;
	};
}

