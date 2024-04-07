#pragma once

#include "Adran/Core/Core.h"
#include "Layer.h"

namespace Adran {
	class AR_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	private:
		std::vector<Layer*> m_layers;
		//iterator�ǵ�����,ʵ�ֶ����ݽṹ��abstraction,��coder�����˽����ݽṹ�ľ���Ϳ��Ա���Ԫ��,��begin��end
		//λ������
		unsigned int m_layerInsertIndex = 0;
	};
}


