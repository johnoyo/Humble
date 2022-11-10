#pragma once
#include "HumbleAPI.h"
#include "IEntity.h"
#include "GlobalArrays.h"

#include <functional>
#include <type_traits>

namespace HBL {

	class HBL_API IRegistrySystem 
	{
	public:
		virtual void Start() { }
		virtual void Run(float dt) { }
		virtual void Clear() { }
		
		template<typename... Ts>
		struct Caller
		{
			static void Call(IEntity& entt) { /*std::cout << "Recursion still going." << std::endl;*/ } // Base case, terminates recursion
		};

		template<typename T, typename... Ts>
		struct Caller<T, Ts...>
		{
			static void Call(IEntity& entt)
			{
				// Do something with T
				//std::cout << "Recursion still going." << std::endl;
				m_Recursions++;

				if (Globals::s_Registry.HasComponent<T>(entt))
					m_ComponentCounter++;

				Caller<Ts...>::Call(entt);
			}
		};

		template<typename... Ts>
		IRegistrySystem& Filter()
		{
			m_FilteredEntities.clear();

			for (IEntity& entt : Globals::s_Registry.GetEntities())
			{
				m_ComponentCounter = 0;
				m_Recursions = 0;

				Caller<Ts...>::Call(entt);

				// If we found all the components requested filter entt.
				if (m_ComponentCounter == m_Recursions)
					m_FilteredEntities.emplace_back(entt);
			}

			return *this;
		}

		// TODO: Iterate through components not entities.
		IRegistrySystem& ForEach(std::function<void(IEntity&)> func)
		{
			m_Function = func;
			return *this;
		}

		void Run()
		{
			for (IEntity& entt : m_FilteredEntities)
			{
				m_Function(entt);
			}
		}

		void Scedule()
		{
			// TODO: Parallel run the m_Function.
			for (IEntity& entt : m_FilteredEntities)
			{
				m_Function(entt);
			}
		}

		void Clean()
		{
			m_Function = nullptr;
			m_FilteredEntities.clear();
		}

	private:
		std::function<void(IEntity&)> m_Function = nullptr;
		std::vector<IEntity> m_FilteredEntities;

		inline static uint32_t m_Recursions = 0;
		inline static uint32_t m_ComponentCounter = 0;
	};

}