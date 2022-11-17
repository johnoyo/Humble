#pragma once

#include "UUID.h"
#include "IEntity.h"
#include "Registry.h"
#include "Components.h"

#include "../HumbleAPI.h"
#include "../Utilities.h"

#include <utility>
#include <iostream>
#include <execution>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace HBL
{
	template<typename... Ts>
	class Group
	{
	public:
		Group(const Group&) = delete;

		static Group& Get()
		{
			static Group instance;
			return instance;
		}

		template <typename... Ts>
		typename std::enable_if<sizeof...(Ts) == 0>::type Caller(IEntity& entt)
		{
			m_cachedHashCode = true;
		}

		template <typename T, typename... Ts>
		void Caller(IEntity& entt)
		{
			m_Recursions++;

			if (Registry::Get().HasComponent<T>(entt))
			{
				if (!m_cachedHashCode)
					m_HashCodes[m_HashCodes.size() - 1]->push_back(typeid(T).hash_code());

				m_ComponentCounter++;
			}

			Caller<Ts...>(entt);
		}

		// TODO: Iterate through components not entities.
		Group& ForEach(std::function<void(IEntity&)> func)
		{
			m_FilteredEntities.clear();
			m_FunctionFilter = nullptr;
			m_cachedHashCode = false;

			if (!CachedRelationship())
			{
				m_HashCodes.push_back(new std::vector<std::size_t>());

				for (IEntity& entt : Registry::Get().GetEntities())
				{
					m_ComponentCounter = 0;
					m_Recursions = 0;

					Caller<Ts...>(entt);

					// If we found all the components requested filter entt.
					if (m_ComponentCounter == m_Recursions)
						m_FilteredEntities.emplace_back(entt);
				}

				m_AllFilteredEntities.push_back(&m_FilteredEntities);
			}

			m_FunctionFilter = func;
			return *this;
		}

		void Run()
		{
			for (IEntity& entt : m_FilteredEntities)
			{
				m_FunctionFilter(entt);
			}
		}

		void Scedule()
		{
			std::for_each(
				std::execution::par,
				m_FilteredEntities.begin(),
				m_FilteredEntities.end(),
				[&](auto&& item)
				{
					m_FunctionFilter(item);
				});
		}

	private:
		Group() { };

		template <typename... Ts>
		typename std::enable_if<sizeof...(Ts) == 0>::type ICaller() 
		{		
			if (m_ComponentRecursions != m_ComponentsFound)
				m_Index = UINT32_MAX;
		}

		template <typename T, typename... Ts>
		void ICaller()
		{
			m_ComponentRecursions++;

			for (int i = 0; i < m_HashCodes.size(); i++)
			{
				for (int j = 0; j < m_HashCodes[i]->size(); j++)
				{
					if (m_HashCodes[i]->at(j) == typeid(T).hash_code())
					{
						m_Flag = true;
						m_Index = i;
						m_ComponentsFound++;
						break;
					}
				}
				if (m_Flag)
				{
					m_Flag = false;
					break;
				}
			}

			ICaller<Ts...>();
		}

		bool CachedRelationship()
		{
			m_ComponentRecursions = 0;
			m_ComponentsFound = 0;
			m_Flag = false;
			m_Index = UINT32_MAX;

			ICaller<Ts...>();

			if (m_Index == UINT32_MAX)
			{
				return false;
			}

			m_FilteredEntities = *m_AllFilteredEntities[m_Index];
			return true;
		}

		std::function<void(IEntity&)> m_FunctionFilter = nullptr;
		std::vector<IEntity> m_FilteredEntities;
		uint32_t m_Recursions = 0;
		uint32_t m_ComponentCounter = 0;

		uint32_t m_ComponentsFound = 0;
		uint32_t m_ComponentRecursions = 0;
		uint32_t m_Index = 0;
		bool m_cachedHashCode = false;
		bool m_Flag = false;

		std::vector<std::vector<std::size_t>*> m_HashCodes;
		std::vector<std::vector<IEntity>*> m_AllFilteredEntities;
	};
}