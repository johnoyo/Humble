#pragma once
#include <vector>
#include "Managers/Entities.h"
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
			for (IEntity& entt : filtered) {
				func(entt);
			}
		}

		void Filter(std::vector<IEntity> current_entities)
		{
			return;
		}

		template<typename T, typename... Ts>
		void Filter(std::vector<IEntity> current_entities, T& param, Ts&... params)
		{
			FUNCTION_PROFILE();
			filtered.clear();

			auto lamda = [&](IEntity entt)
			{
				return entt.components.find(param) != entt.components.end();
			};

			for (
				auto it = std::find_if(current_entities.begin(), current_entities.end(), lamda);
				it != current_entities.end();
				it = std::find_if(++it, current_entities.end(), lamda)
				)
			{
				filtered.push_back(*it);
			}

			Filter(filtered, params...);
		}

	private:
		std::vector<IEntity> filtered;
	};

}