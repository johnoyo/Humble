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
		Registry::Get().RegisterSystem(&GlobalSystems::spriteRendererSystem);
		Registry::Get().RegisterSystem(&GlobalSystems::animationSystem);
		Registry::Get().RegisterSystem(&GlobalSystems::scriptingSystem);
		Registry::Get().RegisterSystem(&GlobalSystems::transformSystem);
		Registry::Get().RegisterSystem(&GlobalSystems::gravitySystem);
		Registry::Get().RegisterSystem(&GlobalSystems::collisionSystem);
		Registry::Get().RegisterSystem(&GlobalSystems::textSystem);
	}

	void Application::RegisterArrays()
	{
		Registry::Get().AddArray<Component::Transform>();
		Registry::Get().AddArray<Component::SpriteRenderer>();
		Registry::Get().AddArray<Component::CollisionBox>();
		Registry::Get().AddArray<Component::Script>();
		Registry::Get().AddArray<Component::Gravity>();
		Registry::Get().AddArray<Component::Animation>();
		Registry::Get().AddArray<Component::Shadow>();
		Registry::Get().AddArray<Component::Text>();
		Registry::Get().AddArray<Component::TextTransform>();
	}

	void Application::Add_Scene(IScene* scene)
	{
		m_Scenes.push_back(scene);
	}

}
