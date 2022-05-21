#pragma once
#include "../../Renderer.h"

class InputSystem {
public:
	/* Returns true i.e. 1, as long as the specified key is pressed or released */
	int GetKeyDown(GLFWwindow* window, int Key_Code, int Mode);
	/* Returns true i.e. 1, only the moment that the key is pressed */
	int GetKeyPress(GLFWwindow* window, int Key_Code);
	/* Returns true i.e. 1, only the moment that the key is released */
	int GetKeyRelease(GLFWwindow* window, int Key_Code);
private:
	int last_state_p[349] = { GLFW_RELEASE };
	int last_state_r[349] = { GLFW_PRESS };

	int Check_State(GLFWwindow* window, int Key_Code);
};