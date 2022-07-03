#include "ScriptingSystem.h"

namespace HBL {

	void ScriptingSystem::Start(int current_level)
	{
		FUNCTION_PROFILE();

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

	void ScriptingSystem::Run(int current_level)
	{
		//FUNCTION_PROFILE();

		for (uint32_t i = 0; i < Globals::Script.size(); i++) {
			if (Globals::Script.at(i).Enabled) {
				int size = Globals::Script.at(i).script.size();
				if (current_level < size)
					Globals::Script.at(i).script[current_level]->Update();
				else
					Globals::Script.at(i).script[size - 1]->Update();
			}
		}
	}

	void ScriptingSystem::Clear(int current_level)
	{
		for (uint32_t i = 0; i < Globals::Script.size(); i++) {
			for (uint32_t j = 0; j < current_level; j++) {
				delete Globals::Script.at(i).script[j];
			}
		}

		Globals::Script.clear();
	}

}
