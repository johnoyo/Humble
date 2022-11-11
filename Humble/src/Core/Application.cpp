#include "Application.h"

namespace HBL {

	Application::Application(float width, float height, const std::string& name, bool full_screen, bool vSync, bool fixedTimeStep, float fixedUpdates) : m_FixedTimeStep(fixedTimeStep), m_LimitFPS(1.0f / fixedUpdates)
	{
		RegisterSystems();
		RegisterArrays();

		GlobalSystems::windowSystem.Initialize(width, height, name, full_screen, vSync);
	}

	void Application::RegisterSystems()
	{
		Globals::s_Registry.RegisterSystem(&GlobalSystems::spriteRendererSystem);
		Globals::s_Registry.RegisterSystem(&GlobalSystems::animationSystem);
		Globals::s_Registry.RegisterSystem(&GlobalSystems::scriptingSystem);
		Globals::s_Registry.RegisterSystem(&GlobalSystems::transformSystem);
		Globals::s_Registry.RegisterSystem(&GlobalSystems::gravitySystem);
		Globals::s_Registry.RegisterSystem(&GlobalSystems::collisionSystem);
		Globals::s_Registry.RegisterSystem(&GlobalSystems::textSystem);
	}

	void Application::RegisterArrays()
	{
		Globals::s_Registry.AddArray<Component::Transform>();
		Globals::s_Registry.AddArray<Component::SpriteRenderer>();
		Globals::s_Registry.AddArray<Component::CollisionBox>();
		Globals::s_Registry.AddArray<Component::Script>();
		Globals::s_Registry.AddArray<Component::Gravity>();
		Globals::s_Registry.AddArray<Component::Animation>();
		Globals::s_Registry.AddArray<Component::Shadow>();
		Globals::s_Registry.AddArray<Component::Text>();
		Globals::s_Registry.AddArray<Component::TextTransform>();
	}

	void Application::Add_Scene(IScene* scene)
	{
		m_Scenes.push_back(scene);
	}

}
