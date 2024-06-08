#pragma once
#include <Adran.h>
#include"ParticleSystem.h"

namespace Adran
{

	class pingtuLayer : public Layer
	{
	public:
		struct Slot
		{
			int i;
			int j;
			glm::vec2 position;
			Entity entity;
			bool isLive = true;

			bool operator==(const Slot& slot)
			{
				if (i == slot.i && j == slot.j)
				{
					return true;
				}
				return false;
			}
		};

		pingtuLayer();

		virtual ~pingtuLayer() {}

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(TimeStep ts) override;
		void OnEvent(Event& event) override;

		void OnImGuiRender() override;

	private:
		
		//Scene
		Ref<Scene> m_scene;
	private:
		uint32_t m_verticalPi = 4;
		uint32_t m_horizalPi = 3;
		Ref<Texture2D> m_orginTex;
		std::vector<Slot> m_subEntities;
		std::vector<Slot> m_slotEntities;
		std::vector<Ref<SubTexture>> m_subTexes;

		float m_xOffset;
		float m_yOffset;
		
		glm::vec2 m_mouse_delta = { 0.0f, 0.0f };
		Slot* m_handleSlot = nullptr;
	};
}
