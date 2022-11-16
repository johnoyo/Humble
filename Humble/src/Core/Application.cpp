#include "Application.h"

namespace HBL {

	Application::Application(float width, float height, const std::string& name, bool full_screen, bool vSync, bool fixedTimeStep, float fixedUpdates) : m_FixedTimeStep(fixedTimeStep), m_LimitFPS(1.0f / fixedUpdates)
	{
		RegisterSystems();
		RegisterArrays();

		Systems::Window.Initialize(width, height, name, full_screen, vSync);
	}

	void Application::ManageScenes()
	{
		if (Globals::Scene_Change) 
		{
			if (m_Current < m_Scenes.size() - 1)
			{
				Globals::Scene_Change = false;
				Globals::Current_Level++;
				m_Current++;

				// Clear Systems and ECS
				Clear();

				m_Scenes[m_Current]->EnrollEntities();
				m_Scenes[m_Current]->AddComponents();
				m_Scenes[m_Current]->InitComponents();

				// Initialize Systems
				RestartSystems();
			}
		}
	}

	void Application::Start()
	{
		m_Scenes[m_Current]->EnrollEntities();
		m_Scenes[m_Current]->AddComponents();
		m_Scenes[m_Current]->InitComponents();

		InitializeSystems();

		while (!Systems::Window.WindowShouldClose())
		{
			// Measure time and delta time
			float time = (float)glfwGetTime();
			m_DeltaTime = time - m_LastTime;
			m_FixedDeltaTime += (time - m_LastTime) / m_LimitFPS;
			m_LastTime = time;

			// Manage scenes and level switching 
			ManageScenes();

			// Update Registered Systems
			UpdateSystems(m_DeltaTime);

			// Update sound system
			SoundManager::Update();

			// Render
			Renderer::Get().Render(Globals::Camera);
			m_Frames++;

			// Reset after one second
			if (glfwGetTime() - m_Timer > 1.0)
			{
				// Display frame rate at window bar
				std::stringstream ss;
				ss << Systems::Window.GetTitle() << " [" << m_Frames << " FPS]";
				glfwSetWindowTitle(Systems::Window.GetWindow(), ss.str().c_str());

				// Log framerate and delta time to console
				ENGINE_LOG("FPS: %d DeltaTime: %f", m_Frames, m_DeltaTime);

				m_Timer++;
				m_Frames = 0;
				m_FixedUpdates = 0;
			}

			Systems::Window.SwapBuffers();
			Systems::Window.PollForEvents();
		}

		Shutdown();
	}

	void Application::RegisterSystems()
	{
		Registry::Get().RegisterSystem(&Systems::SpriteRenderer);
		Registry::Get().RegisterSystem(&Systems::Animation);
		Registry::Get().RegisterSystem(&Systems::Scripting);
		Registry::Get().RegisterSystem(&Systems::Transform);
		Registry::Get().RegisterSystem(&Systems::Camera);
		Registry::Get().RegisterSystem(&Systems::Gravity);
		Registry::Get().RegisterSystem(&Systems::Collision);
		Registry::Get().RegisterSystem(&Systems::Text);
		Registry::Get().RegisterSystem(&Systems::Shadow);
	}

	void Application::RegisterArrays()
	{
		Registry::Get().AddArray<Component::Camera>();
		Registry::Get().AddArray<Component::Tag>();
		Registry::Get().AddArray<Component::Transform>();
		Registry::Get().AddArray<Component::SpriteRenderer>();
		Registry::Get().AddArray<Component::CollisionBox>();
		Registry::Get().AddArray<Component::Script>();
		Registry::Get().AddArray<Component::Gravity>();
		Registry::Get().AddArray<Component::Animation>();
		Registry::Get().AddArray<Component::Shadow>();
		Registry::Get().AddArray<Component::Text>();
		Registry::Get().AddArray<Component::TextTransform>();
		Registry::Get().AddArray<Component::Shadow>();
	}

	void Application::InitializeSystems()
	{
		Systems::Window.Create();

		SoundManager::Start();

		for (ISystem* system : Registry::Get().GetSystems())
		{
			system->Start();
		}
	}

	void Application::RestartSystems()
	{
		for (ISystem* system : Registry::Get().GetSystems())
		{
			system->Start();
		}
	}

	void Application::UpdateFixedSystems(float dt)
	{
		// - Only update at specified frames / s
		while (m_FixedDeltaTime >= 1.0f)
		{
			// Update fixed systems
			for (ISystem* system : Registry::Get().GetSystems())
			{
				system->Run(dt);
			}

			m_FixedUpdates++;
			m_FixedDeltaTime--;
		}
	}

	void Application::UpdateSystems(float dt)
	{
		if (!m_FixedTimeStep)
		{
			for (ISystem* system : Registry::Get().GetSystems())
			{
				system->Run(dt);
			}
		}
		else
		{
			UpdateFixedSystems(m_LimitFPS);
		}
	}

	void Application::Clear()
	{
		for (ISystem* system : Registry::Get().GetSystems())
		{
			system->Clear();
			system->Clean();
		}

		Renderer::Get().Clear();
		Registry::Get().Flush();
	}

	void Application::Shutdown()
	{
		Clear();

		SoundManager::Clean();
		Systems::Window.Terminate();
	}

	void Application::AddScene(IScene* scene)
	{
		m_Scenes.push_back(scene);
	}

}
