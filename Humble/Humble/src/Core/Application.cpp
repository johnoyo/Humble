#include "Application.h"

namespace HBL {

	Application::Application(float width, float height, const std::string& name, bool full_screen, bool vSync)
	{
		GlobalSystems::windowSystem.Initialize(width, height, name, full_screen, vSync);
	}

	void Application::Add_Scene(IScene* scene)
	{
		scenes.push_back(scene);
	}

	void Application::Register_System(ISystem* system)
	{
		systems.push_back(system);
	}

}
