#include "AdranPreCompile.h"
#include "Layer.h"

namespace Adran {

	Layer::Layer(const std::string& name /*= "Default Layer"*/)
	{
		m_name = name;
	}

	Layer::~Layer()
	{

	}

	void Layer::OnAttach()
	{

	}

	void Layer::OnDetach()
	{

	}

	void Layer::OnUpdate(TimeStep ts)
	{

	}

	void Layer::OnImGuiRender()
	{

	}

	void Layer::OnEvent(Event& event)
	{

	}

}