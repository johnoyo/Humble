#include "InputManager.h"
#include "../GlobalSystems.h"

namespace HBL {

	int InputManager::IGetKeyDown(int Key_Code, int Mode)
	{
		GLFWwindow* window = GlobalSystems::windowSystem.Get_Window();

		int result;
		result = (glfwGetKey(window, Key_Code) == Mode);
		return result;
	}

	int InputManager::IGetKeyPress(int Key_Code)
	{
		GLFWwindow* window = GlobalSystems::windowSystem.Get_Window();

		int result = 0;
		if (Check_State(Key_Code) == GLFW_PRESS && last_state_p[Key_Code] == GLFW_RELEASE) result = Get().IGetKeyDown(Key_Code, GLFW_PRESS);
		last_state_p[Key_Code] = Check_State(Key_Code);
		return result;
	}

	int InputManager::IGetKeyRelease(int Key_Code)
	{
		GLFWwindow* window = GlobalSystems::windowSystem.Get_Window();

		int result = 0;
		if (Check_State(Key_Code) == GLFW_RELEASE && last_state_r[Key_Code] == GLFW_PRESS) result = Get().IGetKeyDown(Key_Code, GLFW_RELEASE);
		last_state_r[Key_Code] = Check_State(Key_Code);
		return result;
	}

	int InputManager::Check_State(int Key_Code)
	{
		GLFWwindow* window = GlobalSystems::windowSystem.Get_Window();

		return glfwGetKey(window, Key_Code);
	}

}
