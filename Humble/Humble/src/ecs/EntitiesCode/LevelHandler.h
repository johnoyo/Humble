#pragma once
#include <iostream>
#include "../Header.h"
#include "../Managers/InputManager.h"

namespace LevelHandler {

	namespace Level_0 {

		void init(float dt) {
			std::cout << "Calling level init 0\n";
			LevelManager::Load_Level("res/levels/test3.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background);
		}

		void update(float dt) {
			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_L))
				LevelManager::Load_Level("res/levels/test4.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background);

			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_K))
				LevelManager::Load_Level("res/levels/test2.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background);
		}

	}

	namespace Level_1 {

		void init(float dt) {
			std::cout << "Calling level init 1\n";
		}

		void update(float dt) {
			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_L))
				LevelManager::Load_Level("res/levels/test4.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background);

			if (InputManager::GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_K))
				LevelManager::Load_Level("res/levels/test2.txt", scriptingSystem, gravitySystem, renderingSystem, renderingSystem.Get_Vertex_Buffer(), renderingSystem.Get_Index_Buffer(), background);
		}

	}

}

