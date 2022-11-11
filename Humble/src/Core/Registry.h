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
		Registry(const Registry&) = delete;

		static Registry& Get()
		{
			static Registry instance;
			return instance;
		}

		void EnrollEntity(IEntity& Entity)
		{
			Entity.m_UUID = Random::UInt64(0, UINT64_MAX);
			m_Entities.emplace_back(Entity);
		}

		void EnrollEntityWithUUID(IEntity& Entity, uint64_t uuid)
		{
			Entity.m_UUID = uuid;
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
			m_ComponentArrays.emplace(typeid(T).hash_code(), new std::unordered_map<uint64_t, T>());
		}

		template<typename T>
		std::unordered_map<std::uint64_t, T>& GetArray()
		{
			return *(std::unordered_map<uint64_t, T>*)m_ComponentArrays[typeid(T).hash_code()];
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

		Registry() {};

		std::vector<IEntity> m_Entities;
		std::vector<IRegistrySystem*> m_Systems;
		std::unordered_map<std::size_t, void*> m_ComponentArrays;
	};
}
