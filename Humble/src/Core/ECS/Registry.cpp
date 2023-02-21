#include "Registry.h"

namespace HBL
{
	void Registry::EnrollEntity(IEntity& Entity, const std::string& name)
	{
		Entity.uuid = Random::UInt64(0, UINT64_MAX);
		m_Entities.emplace_back(Entity);

		AddComponent<Component::Transform>(Entity);
		AddComponent<Component::Tag>(Entity);

		if (name.empty())
			GetComponent<Component::Tag>(Entity).tag = "UnNamedEntity";
		else
			GetComponent<Component::Tag>(Entity).tag = name;
	}

	void Registry::EnrollEntityWithUUID(IEntity& Entity, UUID& uuid)
	{
		Entity.uuid = uuid;
		m_Entities.emplace_back(Entity);
	}

	IEntity& Registry::FindEntityWithTag(const std::string& tag)
	{
		for (IEntity& entt : m_Entities)
		{
			Component::Tag& tagComponent = GetComponent<Component::Tag>(entt);

			if (tagComponent.tag == tag)
				return entt;
		}

		ENGINE_LOG("Could not find entity with tag: %s", tag);

		return InvalidEntity;
	}

	std::vector<IEntity> Registry::FindEntitiesWithTag(const std::string& tag)
	{
		std::vector<IEntity> entities;

		for (IEntity& entt : m_Entities)
		{
			Component::Tag& tagComponent = GetComponent<Component::Tag>(entt);

			if (tagComponent.tag == tag)
				entities.emplace_back(entt);
		}

		if (entities.empty())
			ENGINE_LOG("Could not find any entities with tag: %s", tag);

		return entities;
	}
}