#include "InputManager.h"
#include "../Systems.h"

namespace HBL 
{
	int InputManager::IGetKeyDown(int keyCode, int mode)
	{
		GLFWwindow* window = Systems::Window.GetWindow();

		int result;

		if (keyCode >= 0 && keyCode <= 7)
			result = (glfwGetMouseButton(window, keyCode) == mode);
		else
			result = (glfwGetKey(window, keyCode) == mode);

		return result;
	}

	int InputManager::IGetKeyPress(int keyCode)
	{
		GLFWwindow* window = Systems::Window.GetWindow();

		int result = 0;
		if (Check_State(keyCode) == GLFW_PRESS && m_LastPressedState[keyCode] == GLFW_RELEASE)
			result = Get().IGetKeyDown(keyCode, GLFW_PRESS);

		m_LastPressedState[keyCode] = Check_State(keyCode);

		return result;
	}

	int InputManager::IGetKeyRelease(int keyCode)
	{
		GLFWwindow* window = Systems::Window.GetWindow();

		int result = 0;
		if (Check_State(keyCode) == GLFW_RELEASE && m_LastReleasedState[keyCode] == GLFW_PRESS)
			result = Get().IGetKeyDown(keyCode, GLFW_RELEASE);

		m_LastReleasedState[keyCode] = Check_State(keyCode);

		return result;
	}

	glm::vec2 InputManager::IGetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(Systems::Window.GetWindow(), &x, &y);
		return glm::vec2((float)x, (float)y);
	}

	int InputManager::Check_State(int keyCode)
	{
		GLFWwindow* window = Systems::Window.GetWindow();

		if (keyCode >= 0 && keyCode <= 7)
			return glfwGetMouseButton(window, keyCode);
		else
			return glfwGetKey(window, keyCode);
	}
}
