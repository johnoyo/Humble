#pragma once

#include "UUID.h"
#include "IEntity.h"
#include "Components.h"

#include "../Core.h"
#include "../HumbleAPI.h"
#include "../Utilities.h"
#include "../Utilities/Random.h"

#include <iostream>
#include <type_traits>
#include <unordered_map>

namespace HBL 
{
	class IRegistrySystem;

	class HBL_API Registry
	{
	public:
		Registry(const Registry&) = delete;

		static Registry& Get()
		{
			static Registry instance;
			return instance;
		}

		void EnrollEntity(IEntity& Entity, const std::string& name = "")
		{
			Entity.m_UUID = UUID(Random::UInt64(0, UINT64_MAX));
			m_Entities.emplace_back(Entity);

			AddComponent<Component::Tag>(Entity);

			if (name.empty())
				GetComponent<Component::Tag>(Entity).tag = "UnNamedEntity";
			else
				GetComponent<Component::Tag>(Entity).tag = name;
		}

		void EnrollEntityWithUUID(IEntity& Entity, UUID& uuid, const std::string& name)
		{
			Entity.m_UUID = UUID(uuid);
			m_Entities.emplace_back(Entity);

			AddComponent<Component::Tag>(Entity).tag = name;
		}

		template<typename T>
		T& AddComponent(IEntity& Entity)
		{
			//ASSERT(!HasComponent<T>(Entity));

			T component;

			auto& array = GetArray<T>();
			array.emplace(Entity.m_UUID, component);
			
			return component;
		}

		template<typename T>
		T& GetComponent(IEntity& Entity)
		{
			//ASSERT(HasComponent<T>(Entity));

			auto& array = GetArray<T>();
			return array[Entity.m_UUID];
		}

		template<typename T>
		bool HasComponent(IEntity& Entity)
		{
			auto& array = GetArray<T>();
			return (array.find(Entity.m_UUID) != array.end());
		}

		template<typename T>
		void RemoveComponent(IEntity& Entity)
		{
			ASSERT(HasComponent<T>(Entity));
		}

		template<typename T>
		void AddArray()
		{
			std::unordered_map<UUID, T>* array = new std::unordered_map<UUID, T>();
			//array->max_load_factor(0.25);

			m_ComponentArrays.emplace(typeid(T).hash_code(), array);
		}

		template<typename T>
		std::unordered_map<UUID, T>& GetArray()
		{
			auto& array = *(std::unordered_map<UUID, T>*)m_ComponentArrays[typeid(T).hash_code()];
			return array;
		}

		std::vector<IEntity>& GetEntities()
		{
			return m_Entities;
		}

		std::vector<IRegistrySystem*>& GetSystems()
		{
			return m_Systems;
		}

		void Flush()
		{
			m_Entities.clear();
		}

		void RegisterSystem(IRegistrySystem* system)
		{
			m_Systems.push_back(system);
		}

	private:
		Registry() { };
		
		std::vector<IEntity> m_Entities;
		std::vector<IRegistrySystem*> m_Systems;
		std::unordered_map<std::size_t, void*> m_ComponentArrays;
	};

}
