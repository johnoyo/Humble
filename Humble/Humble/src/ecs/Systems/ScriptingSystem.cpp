#include "ScriptingSystem.h"

void ScriptingSystem::Start(int current_level)
{
	ENGINE_PROFILE("ScriptingSystem::Start");

	for (uint32_t i = 0; i < Script.size(); i++) {
		if (Script.at(i).Enabled) {
			int size = Script.at(i).script.size();
			if (current_level < size)
			{
				if (current_level == -1)
					Script.at(i).script[0]->Init();
				else
					Script.at(i).script[current_level]->Init();
			}
			else
				Script.at(i).script[size - 1]->Init();
		}
	}
}

void ScriptingSystem::Run(int current_level)
{
	//ENGINE_PROFILE("ScriptingSystem::Run");

	for (uint32_t i = 0; i < Script.size(); i++) {
		if (Script.at(i).Enabled) {
			int size = Script.at(i).script.size();
			if (current_level < size)
			{
				if (current_level == -1)
					Script.at(i).script[0]->Update();
				else
					Script.at(i).script[current_level]->Update();
			}
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
