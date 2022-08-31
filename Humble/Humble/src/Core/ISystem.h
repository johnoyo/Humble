#pragma once
#include <vector>
#include "Utilities.h"
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API ISystem {
	public:
		virtual void Start() { }
		virtual void Run()   { }
		virtual void Clear() { }

		// TODO: Clear filtered vector on clear

		void For_Each(std::function<void(IEntity&)> func) 
		{
			//FUNCTION_PROFILE();

			for (IEntity& entt : filtered) {
				func(entt);
			}
		}

		void Filter(std::vector<IEntity>& current_entities)
		{
			return;
		}

		template<typename T, typename... Ts>
		void Filter(std::vector<IEntity>& current_entities, T& param, Ts&... params)
		{
			FUNCTION_PROFILE();
			
			filtered.clear();

			auto lamda = [&](IEntity& entt)
			{
				return entt.components.find(param) != entt.components.end();
			};

			for (
				auto it = std::find_if(current_entities.begin(), current_entities.end(), lamda);
				it != current_entities.end();
				it = std::find_if(++it, current_entities.end(), lamda)
				)
			{
				filtered.emplace_back(*it);
			}

			current.clear();
			current.reserve(filtered.size());
			current.insert(current.end(), filtered.begin(), filtered.end());

			Filter(current, params...);
		}

		ISystem Filter(std::vector<std::string> components)
		{
			//ENGINE_PROFILE("NEW FILTER");

			uint32_t index = 0;

			// Check if the current filter is present in the cached queries list.
			for (std::vector<std::string>& query : cachedQueries) 
			{
				// If queries have different sizes they are automatically differnt.
				if (query.size() != components.size())
				{
					index++;
					continue;
				}

				uint32_t count = 0;

				// NOTE: if filter has the same components more than once we have a problem.
				for (std::string& componentQeuried : query)
				{
					for (std::string& component : components)
					{
						if (component == componentQeuried)
							count++;
					}
				}

				if (count == query.size())
				{
					if (cachedIndex != index)
					{
						filtered = cachedEntities[index];
						cachedIndex = index;
					}

					return *this;
				}

				index++;
			}

			uint32_t count = 0;
			bool success = false;

			filtered.clear();

			for (IEntity& entt : Globals::entities)
			{
				// Find if current entity has the requested components.
				for (std::string& component : components)
				{
					if (entt.components.find(component) != entt.components.end())
					{
						count++;
					}
				}

				// If the amount of found components is equal to the requested ones, filter the entity.
				if (count == components.size())
				{
					filtered.push_back(entt);
					success = true;
				}

				// Reset counter
				count = 0;
			}

			// If the filtering is successfull cache the current query.
			if (success)
			{
				cachedQueries.push_back(components);
				cachedEntities.push_back(filtered);
				cachedIndex = cachedQueries.size() - 1;
			}

			return *this;
		}

		void Clean() 
		{
			current.clear();
			filtered.clear();

			for (int i = 0; i < cachedQueries.size(); i++)
			{
				cachedQueries[i].clear();
			}

			cachedQueries.clear();

			for (int i = 0; i < cachedEntities.size(); i++)
			{
				cachedEntities[i].clear();
			}

			cachedEntities.clear();

			cachedIndex = 999999;
		}

	private:

		std::vector<std::vector<std::string>> cachedQueries;
		std::vector<std::vector<IEntity>> cachedEntities;
		uint32_t cachedIndex = 999999;

		std::vector<IEntity> filtered;
		std::vector<IEntity> current;
	};

}