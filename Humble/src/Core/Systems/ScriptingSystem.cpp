#include "ScriptingSystem.h"

namespace HBL {

	void ScriptingSystem::Start()
	{
		FUNCTION_PROFILE();

		int current_level = Globals::Current_Level;

		Filter<Component::Script>().ForEach([&](IEntity& entt) 
		{
			Component::Script& script = Globals::s_Registry.GetComponent<Component::Script>(entt);

			if (script.Enabled) 
			{
				int size = script.script.size();
				if (current_level < size)
					script.script[current_level]->Init();
				else
					script.script[size - 1]->Init();
			}
		}).Run();
	}

	void ScriptingSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		int current_level = Globals::Current_Level;

		ForEach([&](IEntity& entt)
		{
			Component::Script& script = Globals::s_Registry.GetComponent<Component::Script>(entt);

			if (script.Enabled) 
			{
				int size = script.script.size();
				if (current_level < size)
					script.script[current_level]->Update(dt);
				else
					script.script[size - 1]->Update(dt);
			}
		}).Run();
	}

	void ScriptingSystem::Clear()
	{
		FUNCTION_PROFILE();

		int current_level = Globals::Current_Level;

		ForEach([&](IEntity& entt)
		{
			Component::Script& script = Globals::s_Registry.GetComponent<Component::Script>(entt);

			for (uint32_t j = 0; j < current_level; j++) 
			{
				delete script.script[j];
			}
		}).Run();

		Globals::s_Registry.GetArray<Component::Script>().clear();
	}

}
