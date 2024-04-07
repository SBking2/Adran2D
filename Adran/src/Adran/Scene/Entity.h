#pragma once
#include "Scene.h"
#include <Entt.hpp>

namespace Adran
{
	class Entity
	{
	public:
		Entity();
		Entity(entt::entity handle, Scene* secne);
		Entity(const Entity& other) = default;
		~Entity();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AR_CORE_ASSERT(!HasComponent<T>(), "Entity�Ѿ��и������!");
			return m_scene->Reg().emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			AR_CORE_ASSERT(HasComponent<T>(), "Entityû�и������!");
			return m_scene->Reg().get<T>(m_entityHandle);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_scene->Reg().all_of<T>(m_entityHandle);
		}
		template<typename T>
		void RemoveComponent()
		{
			AR_CORE_ASSERT(HasComponent<T>(), "Entityû�и������!");
			m_scene->Reg().remove<T>(m_entityHandle);
		}

		operator bool() const
		{
			return m_entityHandle != entt::null;
		}

		operator uint32_t() const
		{
			return (uint32_t)m_entityHandle;
		}

		operator entt::entity() const
		{
			return m_entityHandle;
		}

		bool operator==(const Entity& entity)
		{
			if (m_scene == entity.m_scene && m_entityHandle == entity.m_entityHandle)
			{
				return true;
			}
			return false;
		}

		bool operator!=(const Entity& entity)
		{
			if (entity.m_entityHandle != m_entityHandle)
			{
				return true;
			}
			return false;
		}

		auto GetEntityHandle()
		{
			return m_entityHandle;
		}
	private:
		entt::entity m_entityHandle;
		//����ָ�������entity��sence,����Ref<Sence>����Ϊ��ϣ����ϵͳ����Sence��Delete
		Scene* m_scene;
	};
}