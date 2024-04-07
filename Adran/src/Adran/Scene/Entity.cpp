#include "AdranPreCompile.h"
#include "Entity.h"
namespace Adran
{
	Entity::Entity()
	{
		m_scene = nullptr;
		m_entityHandle = entt::null;
	}

	Entity::Entity(entt::entity handle, Scene* secne)
	{
		m_entityHandle = handle;
		m_scene = secne;
	}

	Entity::~Entity()
	{

	}
}