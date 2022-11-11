#pragma once
#include "HumbleAPI.h"
#include "IEntity.h"
#include "GlobalArrays.h"

#include <functional>
#include <type_traits>

namespace HBL {

	template<typename T>
	class IView
	{
	public:
		IView(std::unordered_map<uint64_t, T> viewedComponent)
			: m_ViewedComponent(viewedComponent) { }

		IView& ForEach(std::function<void(T&)> func)
		{
			m_FunctionView = func;
			return *this;
		}

		void Run()
		{
			for (auto& component : m_ViewedComponent)
			{
				m_FunctionView(component.second);
			}
		}

		void Scedule()
		{
			// TODO: Parallel run the m_FunctionFilter.
			for (auto& component : m_ViewedComponent)
			{
				m_FunctionView(component.second);
			}
		}
	private:
		std::unordered_map<uint64_t, T> m_ViewedComponent;
		std::function<void(T&)> m_FunctionView = nullptr;
	};

	class HBL_API IRegistrySystem 
	{
	public:
		virtual void Start() { }
		virtual void Run(float dt) { }
		virtual void Clear() { }
		
		template<typename... Ts>
		struct Caller
		{
			static void Call(IEntity& entt) { } // Base case, terminates recursion
		};

		template<typename T, typename... Ts>
		struct Caller<T, Ts...>
		{
			static void Call(IEntity& entt)
			{
				m_Recursions++;

				if (Registry::Get().HasComponent<T>(entt))
					m_ComponentCounter++;

				Caller<Ts...>::Call(entt);
			}
		};

		template<typename... Ts>
		IRegistrySystem& Filter()
		{
			m_FilteredEntities.clear();

			for (IEntity& entt : Registry::Get().GetEntities())
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
			// TODO: Parallel run the m_Function.
			for (IEntity& entt : m_FilteredEntities)
			{
				m_FunctionFilter(entt);
			}
		}

		void Clean()
		{
			m_FunctionFilter = nullptr;
			m_FilteredEntities.clear();
		}

	private:

		std::function<void(IEntity&)> m_FunctionFilter = nullptr;
		std::vector<IEntity> m_FilteredEntities;
		inline static uint32_t m_Recursions = 0;
		inline static uint32_t m_ComponentCounter = 0;
	};

}