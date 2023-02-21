#include "Application.h"

namespace HBL {

	Application::Application(float width, float height, const std::string& name, bool full_screen, bool vSync, bool fixedTimeStep, float fixedUpdates) : m_FixedTimeStep(fixedTimeStep), m_LimitFPS(1.0f / fixedUpdates)
	{
		RegisterSystems();
		RegisterArrays();

		Systems::Window.Initialize(width, height, name, full_screen, vSync);

		if (m_Scenes.size() != 0)
			SceneManager::Get().m_ActiveScene = m_Scenes[SceneManager::Get().m_CurrentScene];
	}

	void Application::ManageScenes()
	{
		if (SceneManager::Get().m_SceneChange)
		{
			if (SceneManager::Get().m_CurrentScene < m_Scenes.size() - 1 || SceneManager::Get().m_ArbitrarySceneChange)
			{
				m_Scenes[SceneManager::Get().m_CurrentScene]->OnDetach();

				SceneManager::Get().m_SceneChange = false;

				if (SceneManager::Get().m_ArbitrarySceneChange)
				{
					assert(SceneManager::Get().m_NextScene < m_Scenes.size());

					SceneManager::Get().m_ArbitrarySceneChange = false;
					SceneManager::Get().m_CurrentScene = SceneManager::Get().m_NextScene;
				}
				else
				{
					SceneManager::Get().m_CurrentScene++;
				}

				SceneManager::Get().m_ActiveScene = m_Scenes[SceneManager::Get().m_CurrentScene];

				// Clear Systems and ECS
				Clear();

				m_Scenes[SceneManager::Get().m_CurrentScene]->OnAttach();

				// Initialize Systems
				RestartSystems();

				m_Scenes[SceneManager::Get().m_CurrentScene]->OnCreate();
			}
		}
	}

	void Application::Start()
	{
		m_Scenes[SceneManager::Get().m_CurrentScene]->OnAttach();

		InitializeSystems();

		m_Scenes[SceneManager::Get().m_CurrentScene]->OnCreate();

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
			Renderer::Get().Render(SceneManager::Get().m_MainCamera);
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
		Registry::Get().RegisterSystem(&Systems::Clickable);
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
		Registry::Get().AddArray<Component::Text>();
		Registry::Get().AddArray<Component::Shadow>();
		Registry::Get().AddArray<Component::Clickable>();
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
		}

		Renderer::Get().Clear();
		Registry::Get().Clean();
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
