#pragma once

#include "UUID.h"
#include "IEntity.h"
#include "Components.h"

#include "../HumbleAPI.h"
#include "../Utilities.h"
#include "../Utilities/Random.h"

#include <iostream>
#include <execution>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <unordered_map>

namespace HBL 
{
	class ISystem;

	template<typename T>
	class IView
	{
	public:

		IView(const IView&) = delete;

		static IView& Get()
		{
			static IView instance;
			return instance;
		}

		IView& ForEach(std::function<void(T&)> func)
		{
			m_FunctionView = func;
			return *this;
		}

		void Run()
		{
			for (auto&& [uuid, component] : Registry::Get().GetArray<T>())
			{
				m_FunctionView(component);
			}
		}

		void Scedule()
		{
			auto& array = Registry::Get().GetArray<T>();

			std::for_each(
				std::execution::par,
				array.begin(),
				array.end(),
				[&](auto&& item)
				{
					m_FunctionView(item.second);
				});
		}

	private:
		IView() { };

		std::function<void(T&)> m_FunctionView = nullptr;
	};

	class HBL_API Registry
	{
	public:
		Registry(const Registry&) = delete;

		static Registry& Get()
		{
			static Registry instance;
			return instance;
		}

		void EnrollEntity(IEntity& Entity, const std::string& name = "");

		void EnrollEntityWithUUID(IEntity& Entity, UUID& uuid);

		IEntity* FindEntityWithTag(const std::string& tag);

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
			m_ComponentArrays.emplace(typeid(T).hash_code(), array);
		}

		template<typename T>
		std::unordered_map<UUID, T>& GetArray()
		{
			auto& array = *(std::unordered_map<UUID, T>*)m_ComponentArrays[typeid(T).hash_code()];
			return array;
		}

		template<typename T>
		void ClearArray()
		{
			GetArray<T>().clear();
		}

		std::vector<IEntity>& GetEntities()
		{
			return m_Entities;
		}

		std::vector<ISystem*>& GetSystems()
		{
			return m_Systems;
		}

		void Flush()
		{
			m_Entities.clear();
		}

		void RegisterSystem(ISystem* system)
		{
			m_Systems.push_back(system);
		}

		template <typename... Ts>
		Registry& Filter()
		{
			m_FunctionFilter = nullptr;

			if (!CachedRelationship<Ts...>())
			{
				m_HashCodes.push_back(std::vector<std::size_t>());
				m_AllFilteredEntities.push_back(std::vector<IEntity>());
				m_ActiveRelationShips.push_back(true);

				m_Index = m_AllFilteredEntities.size() - 1;

				for (IEntity& entt : Registry::Get().GetEntities())
				{
					m_ComponentCounter = 0;
					m_Recursions = 0;

					Caller<Ts...>(entt);

					// If we found all the components requested, filter entt.
					if (m_ComponentCounter == m_Recursions)
						m_AllFilteredEntities[m_Index].emplace_back(entt);
				}

				if (m_AllFilteredEntities[m_Index].size() == 0)
					m_ActiveRelationShips[m_Index] = false;
			}
			return *this;
		}

		Registry& ForEach(std::function<void(IEntity&)> func)
		{
			m_FunctionFilter = func;
			return *this;
		}

		void Run()
		{
			for (IEntity& entt : m_AllFilteredEntities[m_Index])
			{
				m_FunctionFilter(entt);
			}
		}

		void Scedule()
		{
			std::for_each(
				std::execution::par,
				m_AllFilteredEntities[m_Index].begin(),
				m_AllFilteredEntities[m_Index].end(),
				[&](auto&& item)
				{
					m_FunctionFilter(item);
				});
		}

		void Clean()
		{
			for (int i = 0; i < m_HashCodes.size(); i++)
			{
				m_HashCodes[i].clear();
			}
			m_HashCodes.clear();

			for (int i = 0; i < m_AllFilteredEntities.size(); i++)
			{
				m_AllFilteredEntities[i].clear();
			}
			m_AllFilteredEntities.clear();

			m_ActiveRelationShips.clear();
		}

		template<typename T>
		IView<T>& View()
		{
			return IView<T>::Get();
		}

		template<typename T>
		IView<T>& ForEach(std::function<void(T&)> func)
		{
			return IView<T>::Get().ForEach(func);
		}

	private:

		template <typename... Ts>
		typename std::enable_if<sizeof...(Ts) == 0>::type Caller(IEntity& entt)
		{

		}

		template <typename T, typename... Ts>
		void Caller(IEntity& entt)
		{
			m_Recursions++;

			if (Registry::Get().HasComponent<T>(entt))
			{
				bool exists = false;
				for (auto& code : m_HashCodes[m_HashCodes.size() - 1])
				{
					if (code == typeid(T).hash_code())
					{
						exists = true;
						break;
					}
				}
				if (!exists)
					m_HashCodes[m_HashCodes.size() - 1].push_back(typeid(T).hash_code());

				m_ComponentCounter++;
			}

			Caller<Ts...>(entt);
		}

		template <typename... Ts>
		typename std::enable_if<sizeof...(Ts) == 0>::type ICaller()
		{
			if (m_ComponentRecursions != m_ComponentsFound)
			{
				if (m_ComponentRecursions == m_EmptyRelationShips)
					m_Index = m_EmptyIndex;
				else
					m_Index = UINT32_MAX;
			}
		}

		template <typename T, typename... Ts>
		void ICaller()
		{
			m_ComponentRecursions++;

			for (int i = 0; i < m_HashCodes.size(); i++)
			{
				bool found = false;

				if (!m_ActiveRelationShips[i])
				{
					m_EmptyIndex = i;
					m_EmptyRelationShips++;
					continue;
				}

				for (int j = 0; j < m_HashCodes[i].size(); j++)
				{
					if (m_HashCodes[i][j] == typeid(T).hash_code())
					{
						found = true;
						m_Index = i;
						m_ComponentsFound++;
						break;
					}
				}

				if (found)
				{
					found = false;
					break;
				}
			}

			ICaller<Ts...>();
		}

		template <typename... Ts>
		bool CachedRelationship()
		{
			m_ComponentRecursions = 0;
			m_EmptyRelationShips = 0;
			m_ComponentsFound = 0;

			m_EmptyIndex = UINT32_MAX;
			m_Index = UINT32_MAX;

			ICaller<Ts...>();

			if (m_Index == UINT32_MAX)
			{
				return false;
			}

			return true;
		}

		std::function<void(IEntity&)> m_FunctionFilter = nullptr;
		uint32_t m_Recursions = 0;
		uint32_t m_ComponentCounter = 0;

		uint32_t m_ComponentsFound = 0;
		uint32_t m_ComponentRecursions = 0;
		uint32_t m_EmptyRelationShips = 0;

		uint32_t m_Index = 0;
		uint32_t m_EmptyIndex = 0;

		std::vector<bool> m_ActiveRelationShips;
		std::vector<std::vector<std::size_t>> m_HashCodes;
		std::vector<std::vector<IEntity>> m_AllFilteredEntities;

	private:
		Registry() { };
		
		std::vector<IEntity> m_Entities;
		std::vector<ISystem*> m_Systems;
		std::unordered_map<std::size_t, void*> m_ComponentArrays;
	};
}
