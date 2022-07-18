#pragma once
#include "../Core.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"

namespace HBL {

	class HBL_API InputManager {
	public:
		InputManager(const InputManager&) = delete;

		static InputManager& Get() {
			static InputManager instance;
			return instance;
		}

		// Returns true i.e. 1, as long as the specified key is released
		static int GetKeyUp(int Key_Code) { return Get().IGetKeyDown(Key_Code, GLFW_RELEASE); }
		// Returns true i.e. 1, as long as the specified key is pressed
		static int GetKeyDown(int Key_Code) { return Get().IGetKeyDown(Key_Code, GLFW_PRESS); }
		// Returns true i.e. 1, only the momment that the key is pressed
		static int GetKeyPress(int Key_Code) { return Get().IGetKeyPress(Key_Code); }
		// Returns true i.e. 1, only the momment that the key is released
		static int GetKeyRelease(int Key_Code) { return Get().IGetKeyRelease(Key_Code); }
	private:
		int IGetKeyDown(int Key_Code, int Mode);
		int IGetKeyPress(int Key_Code);
		int IGetKeyRelease(int Key_Code);

		InputManager() {}

		int last_state_p[349] = { GLFW_RELEASE };
		int last_state_r[349] = { GLFW_PRESS };

		int Check_State(int Key_Code);
	};

}
