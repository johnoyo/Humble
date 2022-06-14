#include "ScriptingSystem.h"

namespace HBL {

	void ScriptingSystem::Start(int current_level)
	{
		ENGINE_PROFILE("ScriptingSystem::Start");

		/*ecs.Filter(entities, "Script")->For_Each([&](Entity::BaseEntity entt)
		{
			if (Script.at(entt.Script).Enabled) {
				int size = Script.at(entt.Script).script.size();
				if (current_level < size)
					Script.at(entt.Script).script[current_level]->Init();
				else
					Script.at(entt.Script).script[size - 1]->Init();
			}
		});*/

		for (uint32_t i = 0; i < Script.size(); i++) {
			if (Script.at(i).Enabled) {
				int size = Script.at(i).script.size();
				if (current_level < size)
					Script.at(i).script[current_level]->Init();
				else
					Script.at(i).script[size - 1]->Init();
			}
		}
	}

	void ScriptingSystem::Run(int current_level)
	{
		//ENGINE_PROFILE("ScriptingSystem::Run");

		/*ecs.Filter(entities, "Script")->For_Each([&](Entity::BaseEntity entt)
		{
			if (Script.at(entt.Script).Enabled) {
				int size = Script.at(entt.Script).script.size();
				if (current_level < size)
					Script.at(entt.Script).script[current_level]->Update();
				else
					Script.at(entt.Script).script[size - 1]->Update();
			}
		})*/

		for (uint32_t i = 0; i < Script.size(); i++) {
			if (Script.at(i).Enabled) {
				int size = Script.at(i).script.size();
				if (current_level < size)
					Script.at(i).script[current_level]->Update();
				else
					Script.at(i).script[size - 1]->Update();
			}
		}
	}

	void ScriptingSystem::Clear(int current_level)
	{
		for (uint32_t i = 0; i < Script.size(); i++) {
			for (uint32_t j = 0; j < current_level; j++) {
				delete Script.at(i).script[j];
			}
		}

		Script.clear();
	}

}
