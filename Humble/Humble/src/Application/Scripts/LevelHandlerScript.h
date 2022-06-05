#pragma once

#include "SystemsHeader.h"
#include "../ecs/Managers/InputManager.h"
#include "../ecs/Managers/LevelManager.h"
#include "../ecs/Managers/SoundManager.h"
#include "../ecs/Utilities.h"
#include "../ecs/ScriptFunction.h"

namespace HBL {

	class LevelHandlerScript : public ScriptFunction
	{
	public:
		LevelHandlerScript() {}
		~LevelHandlerScript() {}

		bool once = true;

		virtual void Init() override {
			std::cout << "Calling level init 0\n";
			LevelManager::Load_Level("res/levels/test1.txt");
		}

		virtual void Update() override {

			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_L))
				LevelManager::Load_Level("res/levels/test4.txt");

			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_K))
				LevelManager::Load_Level("res/levels/test2.txt");

		}
	};

}
