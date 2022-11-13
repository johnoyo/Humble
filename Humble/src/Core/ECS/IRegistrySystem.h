#pragma once

#include "IView.h"
#include "Group.h"
#include "IEntity.h"

#include "../HumbleAPI.h"
#include "../GlobalArrays.h"

#include <functional>

namespace HBL 
{
	class HBL_API IRegistrySystem 
	{
	public:
		virtual void Start() { }
		virtual void Run(float dt) { }
		virtual void Clear() { }

		template <typename... Ts>
		IRegistrySystem& Filter()
		{
			m_FilteredEntities.clear();
			m_FunctionFilter = nullptr;

			if (!CachedRelationship<Ts...>())
			{
				m_HashCodes.push_back(std::vector<std::size_t>());

				for (IEntity& entt : Registry::Get().GetEntities())
				{
					m_ComponentCounter = 0;
					m_Recursions = 0;

					Caller<Ts...>(entt);

					// If we found all the components requested filter entt.
					if (m_ComponentCounter == m_Recursions)
						m_FilteredEntities.emplace_back(entt);
				}

				m_AllFilteredEntities.push_back(m_FilteredEntities);
			}
			return *this;
		}

		IRegistrySystem& ForEach(std::function<void(IEntity&)> func)
		{
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
				m_Index = UINT32_MAX;
		}

		template <typename T, typename... Ts>
		void ICaller()
		{
			m_ComponentRecursions++;

			for (int i = 0; i < m_HashCodes.size(); i++)
			{
				bool found = false;

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
			m_ComponentsFound = 0;
			m_Index = UINT32_MAX;

			ICaller<Ts...>();

			if (m_Index == UINT32_MAX)
			{
				return false;
			}

			m_FilteredEntities = m_AllFilteredEntities[m_Index];
			return true;
		}

		template <typename T>
		static std::function<void(T&)> m_FunctionView = nullptr;

		std::function<void(IEntity&)> m_FunctionFilter = nullptr;
		std::vector<IEntity> m_FilteredEntities;
		uint32_t m_Recursions = 0;
		uint32_t m_ComponentCounter = 0;

		uint32_t m_ComponentsFound = 0;
		uint32_t m_ComponentRecursions = 0;
		uint32_t m_Index = 0;
		bool m_cachedHashCode = false;

		std::vector<std::vector<std::size_t>> m_HashCodes;
		std::vector<std::vector<IEntity>> m_AllFilteredEntities;

	};

}