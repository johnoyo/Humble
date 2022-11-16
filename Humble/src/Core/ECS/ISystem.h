#pragma once

#include "IView.h"
#include "Group.h"
#include "IEntity.h"

#include "../HumbleAPI.h"
#include "../GlobalArrays.h"

#include <functional>

namespace HBL 
{
	class HBL_API ISystem 
	{
	public:
		virtual void Start() { }
		virtual void Run(float dt) { }
		virtual void Clear() { }

		template <typename... Ts>
		ISystem& Filter()
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

		ISystem& ForEach(std::function<void(IEntity&)> func)
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
					m_Index = 0;
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

				if (!m_ActiveRelationShips[i])
				{
					m_EmptyRelationShips++;
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

		std::vector<bool> m_ActiveRelationShips;
		std::vector<std::vector<std::size_t>> m_HashCodes;
		std::vector<std::vector<IEntity>> m_AllFilteredEntities;
	};
}