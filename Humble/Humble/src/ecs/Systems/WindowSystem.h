#pragma once
#include "../Renderer.h"
#include <string>

class WindowSystem {
public:
	WindowSystem(float w, float h, const std::string &name, bool full_screen);
	WindowSystem() { }

	void Start(int Vsync);
	void Run();

	void Make_Context_Current(int Vsync);
	int Window_Should_Close();
	void Swap_Buffers();
	void Poll_For_Events();
	void Terminate();

	GLFWwindow* Get_Window();
	float Get_Width();
	float Get_Height();
	float Get_RefreshRate();

private:
	void Init();

	GLFWwindow* window;
	std::string title;
	float refresh_rate;
	float width;
	float height;
	bool fullscreen;
};