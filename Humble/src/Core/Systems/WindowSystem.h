#pragma once
#include "../Utilities.h"
#include "../HumbleAPI.h"

#include <string>

namespace HBL 
{
	class HBL_API WindowSystem 
	{
	public:
		void Initialize(float w, float h, const std::string& name, bool fullScreen, bool VSync);
		void Create();

		void MakeContextCurrent(int Vsync);
		int WindowShouldClose();
		void SwapBuffers();
		void PollForEvents();
		void Terminate();
		void Close();

		GLFWwindow* GetWindow();
		float GetWidth();
		float GetHeight();
		float GetRefreshRate();
		const std::string& GetTitle();
		double GetTime();

	private:
		void Init();

		GLFWwindow* m_Window;
		std::string m_Title;
		float m_RefreshRate;
		float m_Width;
		float m_Height;
		bool m_FullScreen;
		bool m_VSync;
	};

}