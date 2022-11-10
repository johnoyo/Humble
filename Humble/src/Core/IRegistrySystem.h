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
		IRegistrySystem();
		
		IRegistrySystem& Filter(std::vector<std::type_info> components)
		{
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

	private:
		std::function<void(IEntity&)> m_Function = nullptr;
		std::vector<IEntity> m_FilteredEntities;
	};

}