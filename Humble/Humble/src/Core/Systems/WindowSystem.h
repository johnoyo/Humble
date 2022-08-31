#pragma once
#include "../Core.h"
#include <string>
#include "../HumbleAPI.h"

namespace HBL {

	class HBL_API WindowSystem {
	public:
		void Initialize(float w, float h, const std::string& name, bool full_screen);
		void Create(int Vsync);

		void Make_Context_Current(int Vsync);
		int Window_Should_Close();
		void Swap_Buffers();
		void Poll_For_Events();
		void Terminate();

		GLFWwindow* Get_Window();
		float Get_Width();
		float Get_Height();
		float Get_RefreshRate();
		const std::string& Get_Title();

	private:
		void Init();

		GLFWwindow* window;
		std::string title;
		float refresh_rate;
		float width;
		float height;
		bool fullscreen;
	};

}