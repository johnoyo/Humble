#include "InputSystem.h"

int InputSystem::GetKeyDown(GLFWwindow* window, int Key_Code, int Mode)
{
	int result;
	result = (glfwGetKey(window, Key_Code) == Mode);
	return result;
}

int InputSystem::GetKeyPress(GLFWwindow* window, int Key_Code)
{
	int result = 0;
	if (Check_State(window, Key_Code) == GLFW_PRESS && last_state_p[Key_Code] == GLFW_RELEASE) result = GetKeyDown(window, Key_Code, GLFW_PRESS);
	last_state_p[Key_Code] = Check_State(window, Key_Code);
	return result;
}

int InputSystem::GetKeyRelease(GLFWwindow* window, int Key_Code)
{
	int result = 0;
	if (Check_State(window, Key_Code) == GLFW_RELEASE && last_state_r[Key_Code] == GLFW_PRESS) result = GetKeyDown(window, Key_Code, GLFW_RELEASE);
	last_state_r[Key_Code] = Check_State(window, Key_Code);
	return result;
}

int InputSystem::Check_State(GLFWwindow* window, int Key_Code)
{
	return glfwGetKey(window, Key_Code);
}
