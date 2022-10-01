#include "Application.h"

namespace HBL {

	Application::Application(float width, float height, const std::string& name, bool full_screen, bool vSync, bool fixedTimeStep, float fixedUpdates) : m_FixedTimeStep(fixedTimeStep), m_LimitFPS(1.0f / fixedUpdates)
	{
		GlobalSystems::windowSystem.Initialize(width, height, name, full_screen, vSync);
	}

	void Application::Add_Scene(IScene* scene)
	{
		m_Scenes.push_back(scene);
	}

	void Application::Register_System(ISystem* system)
	{
		m_Systems.push_back(system);
	}

}
