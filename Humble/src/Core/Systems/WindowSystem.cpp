#include "WindowSystem.h"

namespace HBL 
{
	void WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void WindowSystem::Initialize(float w, float h, const std::string& name, bool fullScreen, bool VSync)
	{
		m_Width = w;
		m_Height = h;
		m_Title = name;
		m_FullScreen = fullScreen;
		m_VSync = VSync;
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

		m_RefreshRate = mode->refreshRate;
		std::cout << mode->refreshRate << "\n";

		if (m_FullScreen) 
		{
			m_Window = glfwCreateWindow(mode->width, mode->height, m_Title.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else 
		{
			m_Window = glfwCreateWindow((int)m_Width, (int)m_Height, m_Title.c_str(), NULL, NULL);
		}

		if (!m_Window) 
		{
			std::cout << "Error Creating window!!!\n";
			Terminate();
			exit(-1);
		}

		if (m_VSync)
			MakeContextCurrent(1);
		else 
			MakeContextCurrent(0);

		if (glewInit() != GLEW_OK) 
		{
			std::cout << "Error initializing GLEW\n";
		}

		glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
	}

	void WindowSystem::MakeContextCurrent(int Vsync)
	{
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(Vsync);
	}

	int WindowSystem::WindowShouldClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void WindowSystem::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

	void WindowSystem::PollForEvents()
	{
		glfwPollEvents();
	}

	void WindowSystem::Terminate()
	{
		glfwTerminate();
	}

	void WindowSystem::Close()
	{
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
	}

	GLFWwindow* WindowSystem::GetWindow()
	{
		return m_Window;
	}

	float WindowSystem::GetWidth()
	{
		return m_Width;
	}

	float WindowSystem::GetHeight()
	{
		return m_Height;
	}

	float WindowSystem::GetRefreshRate()
	{
		return m_RefreshRate;
	}

	const std::string& WindowSystem::GetTitle()
	{
		return m_Title;
	}

	double WindowSystem::GetTime()
	{
		return glfwGetTime();
	}
}