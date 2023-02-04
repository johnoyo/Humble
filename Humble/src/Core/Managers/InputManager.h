#pragma once

#include "../Utilities.h"
#include "../HumbleAPI.h"

namespace HBL 
{
	class HBL_API InputManager {
	public:
		InputManager(const InputManager&) = delete;

		static InputManager& Get() 
		{
			static InputManager instance;
			return instance;
		}

		static glm::vec2& GetMousePosition() { return Get().IGetMousePosition(); }

		// Returns true i.e. 1, as long as the specified key is released
		static int GetKeyUp(int keyCode) { return Get().IGetKeyDown(keyCode, GLFW_RELEASE); }
		// Returns true i.e. 1, as long as the specified key is pressed
		static int GetKeyDown(int keyCode) { return Get().IGetKeyDown(keyCode, GLFW_PRESS); }
		// Returns true i.e. 1, only the momment that the key is pressed
		static int GetKeyPress(int keyCode) { return Get().IGetKeyPress(keyCode); }
		// Returns true i.e. 1, only the momment that the key is released
		static int GetKeyRelease(int keyCode) { return Get().IGetKeyRelease(keyCode); }

	private:
		int IGetKeyDown(int keyCode, int mode);
		int IGetKeyPress(int keyCode);
		int IGetKeyRelease(int keyCode);

		glm::vec2& IGetMousePosition();

		InputManager() {}

		int m_LastReleasedState[349] = { GLFW_RELEASE };
		int m_LastPressedState[349] = { GLFW_PRESS };
		glm::vec2 m_MousePosition = {};

		int Check_State(int keyCode);
	};
}
