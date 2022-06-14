#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include <algorithm>

namespace HBL {

	class ScriptingSystem {
	public:
		void Start(int current_level);
		void Run(int current_level);
		void Clear(int current_level);
	};

}

/*

class Entities {
public:
	std::vector<BaseEntity> entities;
	std::vector<Entity::BaseEntity>::iterator it;

	std::vector<Entity::BaseEntity>::iterator
}

//////////////////////////////////////////////////////

class ScriptingSystem : ISystems {

public:
	void OnStart() override
	{
		ecs.Entities.Filter(Script).ForEach( [](Script script) => 
		{

		});
	}

	void OnUpdate() override
	{
		ecs.Entities.Filter(Script).ForEach( [](Script script) => 
		{

		});
	}
}


		std::vector<Entity::BaseEntity> filtered_v;

		auto lamda = [](Entity::BaseEntity entt) {
			return entt.Script != -1;
		};

		for (
			auto it = std::find_if(entities.begin(), entities.end(), lamda);
			it != entities.end();
			it = std::find_if(++it, entities.end(), lamda)
			)
		{
			// ...
			std::cout << "\n\nEntities with script: " << (*it).ID << std::endl;
			filtered_v.push_back(*it);
		}


*/
