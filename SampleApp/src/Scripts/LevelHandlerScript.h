#pragma once

#include "LevelManager.h"

namespace HBL 
{
	class LevelHandlerScript final : public IScript
	{
	public:
		LevelHandlerScript() {}
		~LevelHandlerScript() {}

		virtual void OnCreate() override {
			std::cout << "Calling level init 0\n";
			if (LevelManager::GetCurrentLevel() == -1)
				LevelManager::Load_Level("res/levels/test1.txt", true);
			else if (LevelManager::GetCurrentLevel() == 0)
				LevelManager::Load_Level("res/levels/test2.txt", false);
		}

		virtual void OnUpdate(float dt) override {
			if (InputManager::GetKeyPress(GLFW_KEY_K))
				SceneManager::Get().TriggerSceneChange();

			if (InputManager::GetKeyPress(GLFW_KEY_L))
				LevelManager::Load_Level("res/levels/test4.txt", true);
		}

	};
}
