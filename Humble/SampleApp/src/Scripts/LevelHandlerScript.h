#pragma once

#include "LevelManager.h"

namespace HBL {

	class LevelHandlerScript : public ScriptFunction
	{
	public:
		LevelHandlerScript() {}
		~LevelHandlerScript() {}

		virtual void Init() override {
			std::cout << "Calling level init 0\n";
			if (LevelManager::GetCurrentLevel() == -1)
				LevelManager::Load_Level("res/levels/test1.txt", true);
			else if (LevelManager::GetCurrentLevel() == 0)
				LevelManager::Load_Level("res/levels/test2.txt", false);
		}

		virtual void Update() override {
			if (InputManager::GetKeyPress(GlobalSystems::windowSystem.Get_Window(), GLFW_KEY_K))
				Globals::Scene_Change = true;

			if (InputManager::GetKeyPress(GlobalSystems::windowSystem.Get_Window(), GLFW_KEY_L))
				LevelManager::Load_Level("res/levels/test4.txt", true);
		}

	};

}
