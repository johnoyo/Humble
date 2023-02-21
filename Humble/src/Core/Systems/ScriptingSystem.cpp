#include "ScriptingSystem.h"

namespace HBL 
{
	void ScriptingSystem::Start()
	{
		FUNCTION_PROFILE();

		int current_level = SceneManager::Get().GetCurrentScene();;

		Registry::Get().View<Component::Script>().ForEach([&](Component::Script& script)
		{
			if (script.Enabled) 
			{
				int size = script.script.size();
				if (current_level < size)
					script.script[current_level]->OnCreate();
				else
					script.script[size - 1]->OnCreate();
			}
		}).Run();
	}

	void ScriptingSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		int current_level = SceneManager::Get().GetCurrentScene();

		Registry::Get().View<Component::Script>().ForEach([&](Component::Script& script)
		{
			if (script.Enabled) 
			{
				int size = script.script.size();
				if (current_level < size)
					script.script[current_level]->OnUpdate(dt);
				else
					script.script[size - 1]->OnUpdate(dt);
			}
		}).Run();
	}

	void ScriptingSystem::Clear()
	{
		FUNCTION_PROFILE();

		int current_level = SceneManager::Get().GetCurrentScene();

		Registry::Get().View<Component::Script>().ForEach([&](Component::Script& script)
		{
			for (uint32_t j = 0; j < current_level; j++) 
			{
				delete script.script[j];
			}
		}).Run();

		Registry::Get().ClearArray<Component::Script>();
	}

}
