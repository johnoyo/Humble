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
			array.emplace( Entity.m_UUID, component );

			return component;
		}

		template<typename T>
		T& GetComponent(IEntity& Entity)
		{
			ASSERT(HasComponent<T>(Entity));

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

		std::vector<IEntity>& Group(std::vector<std::size_t> components)
		{
			return std::vector<IEntity>();
		}

		void Flush()
		{
			m_Entities.clear();
			m_ComponentArrays.clear();
		}

	private:
		std::vector<IEntity> m_Entities;
		std::vector<uint32_t> m_EntitiesUUIDs;
		std::unordered_map<std::size_t, void*> m_ComponentArrays;

		// TODO: Remove these functions.
		uint64_t CreateHash(uint16_t& componentID, uint16_t& indexOfArray, uint32_t& indexInArray)
		{
			return (uint64_t)((uint32_t)(componentID) << 16 | (uint32_t)(indexOfArray)) << 32 | (uint64_t)(indexInArray);
		}
		uint32_t RetrieveComponentID(uint64_t& hash)
		{
			return (hash & 0xffff000000000000) >> 48;
		}
		uint32_t RetrieveIndexOfArray(uint64_t& hash)
		{
			return (hash & 0x0000ffff00000000) >> 32;
		}
		uint32_t RetrieveIndexInArray(uint64_t& hash)
		{
			return (hash & 0xffffffff);
		}
	};
}
