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
		//iterator是迭代器,实现对数据结构的abstraction,让coder不必了解数据结构的具体就可以遍历元素,从begin到end
		//位置索引
		unsigned int m_layerInsertIndex = 0;
	};
}


