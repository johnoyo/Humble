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

		virtual void Init() override {

			std::cout << "Calling level init 0\n";
			LevelManager::Load_Level("res/levels/test1.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background, true);

		}

		virtual void Update() override {

			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_L))
				LevelManager::Load_Level("res/levels/test4.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background, false);

			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_K))
				LevelManager::Load_Level("res/levels/test2.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background, false);

		}
	};

}
