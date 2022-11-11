#pragma once

#include "IEntity.h"
#include "Components.h"
#include "HumbleAPI.h"
#include "Core.h"
#include "Utilities/Random.h"

#include <iostream>
#include <vector>
#include <type_traits>

namespace HBL 
{
	class IRegistrySystem;

	class HBL_API Registry
	{
	public:
		Registry();
		~Registry();

		void EnrollEntity(IEntity& Entity)
		{
			Entity.m_UUID = Random::Int(100000000); // TODO: Replace with UUIDs
			m_Entities.emplace_back(Entity);
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
			m_ComponentArrays.emplace(typeid(T).hash_code(), new std::unordered_map<uint32_t, T>());
		}

		template<typename T>
		std::unordered_map<std::uint32_t, T>& GetArray()
		{
			return *(std::unordered_map<uint32_t, T>*)m_ComponentArrays[typeid(T).hash_code()];
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
		std::vector<IEntity> m_Entities;
		std::vector<IRegistrySystem*> m_Systems;
		std::unordered_map<std::size_t, void*> m_ComponentArrays;
	};
}
