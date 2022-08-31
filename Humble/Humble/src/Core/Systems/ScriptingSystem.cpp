#include "ScriptingSystem.h"

namespace HBL {

	void ScriptingSystem::Start()
	{
		FUNCTION_PROFILE();

		int current_level = Globals::Current_Level;

		for (uint32_t i = 0; i < Globals::Script.size(); i++) {
			if (Globals::Script.at(i).Enabled) {
				int size = Globals::Script.at(i).script.size();
				if (current_level < size)
					Globals::Script.at(i).script[current_level]->Init();
				else
					Globals::Script.at(i).script[size - 1]->Init();
			}
		}
	}

	void ScriptingSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		int current_level = Globals::Current_Level;

		for (uint32_t i = 0; i < Globals::Script.size(); i++) {
			if (Globals::Script.at(i).Enabled) {
				int size = Globals::Script.at(i).script.size();
				if (current_level < size)
					Globals::Script.at(i).script[current_level]->Update(dt);
				else
					Globals::Script.at(i).script[size - 1]->Update(dt);
			}
		}
	}

	void ScriptingSystem::Clear()
	{
		FUNCTION_PROFILE();

		int current_level = Globals::Current_Level;

		for (uint32_t i = 0; i < Globals::Script.size(); i++) {
			for (uint32_t j = 0; j < current_level; j++) {
				delete Globals::Script.at(i).script[j];
			}
		}

		Globals::Script.clear();
	}

}
