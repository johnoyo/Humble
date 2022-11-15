#include "WindowSystem.h"

namespace HBL {

	void WindowSystem::Initialize(float w, float h, const std::string& name, bool full_screen, bool v_sync)
	{
		width = w;
		height = h;
		title = name;
		fullscreen = full_screen;
		vSync = v_sync;
	}

	void WindowSystem::Init()
	{
		if (!glfwInit()) {
			std::cout << "Error Initializing window!!!\n";
			exit(-1);
		}
	}

	void WindowSystem::Create()
	{
		Init();

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		refresh_rate = mode->refreshRate;
		std::cout << mode->refreshRate << "\n";

		if (fullscreen) {
			window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else {
			window = glfwCreateWindow((int)width, (int)height, title.c_str(), NULL, NULL);
		}
		if (!window) {
			std::cout << "Error Creating window!!!\n";
			Terminate();
			exit(-1);
		}

		if (vSync)
			MakeContextCurrent(1);
		else 
			MakeContextCurrent(0);

		if (glewInit() != GLEW_OK) {
			std::cout << "Error initializing GLEW\n";
		}
	}

	void WindowSystem::MakeContextCurrent(int Vsync)
	{
		glfwMakeContextCurrent(window);
		glfwSwapInterval(Vsync);
	}

	int WindowSystem::WindowShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void WindowSystem::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	void WindowSystem::PollForEvents()
	{
		glfwPollEvents();
	}

	void WindowSystem::Terminate()
	{
		glfwTerminate();
	}

	GLFWwindow* WindowSystem::GetWindow()
	{
		return window;
	}

	float WindowSystem::GetWidth()
	{
		return width;
	}

	float WindowSystem::GetHeight()
	{
		return height;
	}

	float WindowSystem::GetRefreshRate()
	{
		return refresh_rate;
	}

	const std::string& WindowSystem::GetTitle()
	{
		return title;
	}

}