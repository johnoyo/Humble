#include "InputManager.h"

namespace HBL {

	int InputManager::IGetKeyDown(GLFWwindow* window, int Key_Code, int Mode)
	{
		int result;
		result = (glfwGetKey(window, Key_Code) == Mode);
		return result;
	}

	int InputManager::IGetKeyPress(GLFWwindow* window, int Key_Code)
	{
		int result = 0;
		if (Check_State(window, Key_Code) == GLFW_PRESS && last_state_p[Key_Code] == GLFW_RELEASE) result = GetKeyDown(window, Key_Code, GLFW_PRESS);
		last_state_p[Key_Code] = Check_State(window, Key_Code);
		return result;
	}

	int InputManager::IGetKeyRelease(GLFWwindow* window, int Key_Code)
	{
		int result = 0;
		if (Check_State(window, Key_Code) == GLFW_RELEASE && last_state_r[Key_Code] == GLFW_PRESS) result = GetKeyDown(window, Key_Code, GLFW_RELEASE);
		last_state_r[Key_Code] = Check_State(window, Key_Code);
		return result;
	}

	int InputManager::Check_State(GLFWwindow* window, int Key_Code)
	{
		return glfwGetKey(window, Key_Code);
	}

}
