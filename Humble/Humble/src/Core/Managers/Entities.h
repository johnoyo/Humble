#pragma once
#include <vector>
#include "../Entity.h"

namespace HBL {

	class Entities {
	public:
		Entities(const Entities&) = delete;

		std::vector<Entity::BaseEntity> entities;

		static Entities& Get() {
			static Entities instance;
			return instance;
		}

		static void For_Each(std::function<void(Entity::BaseEntity)> func) 
		{
			Get().IFor_Each(func);
		}

		Entities* Filter(std::vector<Entity::BaseEntity> current_entities)
		{
			return static_cast<Entities*>(&Get());
		}

		template<typename T, typename... Ts>
		Entities* Filter(std::vector<Entity::BaseEntity> current_entities, T param, Ts... params)
		{
			Get().filtered.clear();

			auto lamda = [&](Entity::BaseEntity entt) {
				if (param == "Transform")
					return entt.Transform != -1;
				else if (param == "Material")
					return entt.Material != -1;
				else if (param == "CollisionBox")
					return entt.CollisionBox != -1;
				else if (param == "Shadow")
					return entt.Shadow != -1;
				else if (param == "Gravity")
					return entt.Gravity != -1;
				else if (param == "Script")
					return entt.Script != -1;
				else if (param == "Animation")
					return entt.Animation != -1;
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

		void IFor_Each(std::function<void(Entity::BaseEntity)> func) {
			for (Entity::BaseEntity entt : filtered) {
				func(entt);
			}
			Get().filtered.clear();
		}

		std::vector<Entity::BaseEntity> filtered;

	};

}