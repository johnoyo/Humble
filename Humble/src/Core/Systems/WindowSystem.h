#pragma once
#include "../Utilities.h"
#include "../HumbleAPI.h"

#include <string>

namespace HBL 
{
	class HBL_API WindowSystem 
	{
	public:
		void Initialize(float w, float h, const std::string& name, bool full_screen, bool v_sync);
		void Create();

		void MakeContextCurrent(int Vsync);
		int WindowShouldClose();
		void SwapBuffers();
		void PollForEvents();
		void Terminate();

		GLFWwindow* GetWindow();
		float GetWidth();
		float GetHeight();
		float GetRefreshRate();
		const std::string& GetTitle();

	private:
		void Init();

		GLFWwindow* window;
		std::string title;
		float refresh_rate;
		float width;
		float height;
		bool fullscreen;
		bool vSync;
	};

}