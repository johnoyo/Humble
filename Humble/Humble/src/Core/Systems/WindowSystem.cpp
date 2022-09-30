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
			Make_Context_Current(1);
		else 
			Make_Context_Current(0);

		if (glewInit() != GLEW_OK) {
			std::cout << "Error initializing GLEW\n";
		}
	}

	void WindowSystem::Make_Context_Current(int Vsync)
	{
		glfwMakeContextCurrent(window);
		glfwSwapInterval(Vsync);
	}

	int WindowSystem::Window_Should_Close()
	{
		return glfwWindowShouldClose(window);
	}

	void WindowSystem::Swap_Buffers()
	{
		glfwSwapBuffers(window);
	}

	void WindowSystem::Poll_For_Events()
	{
		glfwPollEvents();
	}

	void WindowSystem::Terminate()
	{
		glfwTerminate();
	}

	GLFWwindow* WindowSystem::Get_Window()
	{
		return window;
	}

	float WindowSystem::Get_Width()
	{
		return width;
	}

	float WindowSystem::Get_Height()
	{
		return height;
	}

	float WindowSystem::Get_RefreshRate()
	{
		return refresh_rate;
	}

	const std::string& WindowSystem::Get_Title()
	{
		return title;
	}

}