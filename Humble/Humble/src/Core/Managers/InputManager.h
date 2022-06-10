#pragma once
#include "../Core.h"

namespace HBL {

	class InputManager {
	public:
		InputManager(const InputManager&) = delete;

		static InputManager& Get() {
			static InputManager instance;
			return instance;
		}

		// Returns true i.e. 1, as long as the specified key is pressed or released
		static int GetKeyDown(GLFWwindow* window, int Key_Code, int Mode) { return Get().IGetKeyDown(window, Key_Code, Mode); }
		// Returns true i.e. 1, only the moment that the key is pressed
		static int GetKeyPress(GLFWwindow* window, int Key_Code) { return Get().IGetKeyPress(window, Key_Code); }
		// Returns true i.e. 1, only the moment that the key is released
		static int GetKeyRelease(GLFWwindow* window, int Key_Code) { return Get().IGetKeyRelease(window, Key_Code); }
	private:
		int IGetKeyDown(GLFWwindow* window, int Key_Code, int Mode);
		int IGetKeyPress(GLFWwindow* window, int Key_Code);
		int IGetKeyRelease(GLFWwindow* window, int Key_Code);

		InputManager() {}

		int last_state_p[349] = { GLFW_RELEASE };
		int last_state_r[349] = { GLFW_PRESS };

		int Check_State(GLFWwindow* window, int Key_Code);
	};

}
