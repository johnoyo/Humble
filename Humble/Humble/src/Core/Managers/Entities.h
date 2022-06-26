#pragma once
#include <vector>
#include "../IEntity.h"

namespace HBL {

	class Entities {
	public:
		Entities(const Entities&) = delete;

		std::vector<IEntity> entities;
		std::vector<IEntity> filtered;

		static Entities& Get() {
			static Entities instance;
			return instance;
		}

		static void For_Each(std::function<void(IEntity&)> func) 
		{
			ENGINE_PROFILE("For_Each");
			Get().IFor_Each(func);
		}

		Entities& Filter(std::vector<IEntity> current_entities)
		{
			return Get();
		}

		template<typename T, typename... Ts>
		Entities& Filter(std::vector<IEntity> current_entities, T& param, Ts&... params)
		{
			ENGINE_PROFILE("Filter: param");
			// TODO: fix this

			Get().filtered.clear();

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
				Get().filtered.push_back(*it);
			}

			return Filter(Get().filtered, params...);
		}

	private:
		Entities() {}

		void IFor_Each(std::function<void(IEntity&)> func) {
			for (IEntity& entt : filtered) {
				func(entt);
			}
			Get().filtered.clear();
		}

		std::vector<IEntity> current;

	};

}