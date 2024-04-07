#pragma once
#include "Entity.h"

namespace Adran
{
	class Script
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

		virtual void OnCreate() = 0;
		virtual void OnUpdate(TimeStep ts) = 0;
		virtual void OnDestory() = 0;

	protected:
		//可以调用所属entity的组件
		Entity m_entity;
		friend class Scene;
	};
}
