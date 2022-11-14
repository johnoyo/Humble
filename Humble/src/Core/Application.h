#pragma once
#include <vector>
#include "GlobalArrays.h"
#include "GlobalSystems.h"
#include "Utilities.h"
#include "HumbleAPI.h"
#include "IScene.h"
#include "ECS/ISystem.h"
#include "ECS/Registry.h"

namespace HBL {

	class HBL_API Application {
	public:
		Application(float width, float height, const std::string& name, bool full_screen, bool vSync, bool fixedTimeStep, float fixedUpdates = 60.0f);

		void Add_Scene(IScene* scene);

		void Manage_Scenes()
		{
			if (Globals::Scene_Change) {
				if (m_Current < m_Scenes.size() - 1)
				{
					Globals::Scene_Change = false;
					Globals::Current_Level++;
					m_Current++;

					// Clear Systems and ECS
					Clear();

					// Initialize Systems
					m_Scenes[m_Current]->Init_Systems();

					m_Scenes[m_Current]->Enroll_Entities();
					m_Scenes[m_Current]->Add_Components();
					m_Scenes[m_Current]->Init_Components();

					// Initialize Systems
					Restart_Systems();
				}
			}
		}

		void Start() {

			m_Scenes[m_Current]->Init_Systems();

			m_Scenes[m_Current]->Enroll_Entities();
			m_Scenes[m_Current]->Add_Components();
			m_Scenes[m_Current]->Init_Components();

			// Initialize Systems
			Initialize_Systems();

			while (!GlobalSystems::windowSystem.Window_Should_Close())
			{
				// Measure time and delta time
				float time = (float)glfwGetTime();
				m_DeltaTime = time - m_LastTime;
				m_FixedDeltaTime += (time - m_LastTime) / m_LimitFPS;
				m_LastTime = time;

				// Manage scenes and level switching 
				Manage_Scenes();

				// Update Registered Systems
				Update_Systems(m_DeltaTime);

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
					ss << GlobalSystems::windowSystem.Get_Title() << " [" << m_Frames << " FPS]";
					glfwSetWindowTitle(GlobalSystems::windowSystem.Get_Window(), ss.str().c_str());

					// Log framerate and delta time to console
					ENGINE_LOG("FPS: %d DeltaTime: %f", m_Frames, m_DeltaTime);

					m_Timer++;
					m_Frames = 0;
					m_FixedUpdates = 0;
				}

				GlobalSystems::windowSystem.Swap_Buffers();
				GlobalSystems::windowSystem.Poll_For_Events();
			}

			Shutdown();
		}

	private:
		std::vector<IScene*> m_Scenes;

		uint32_t m_Current = 0;
		bool m_FixedTimeStep = false;

		void RegisterSystems();

		void RegisterArrays();

		void Initialize_Systems() 
		{
			GlobalSystems::windowSystem.Create();

			Globals::Camera = m_Scenes[m_Current]->GetCamera();

			SoundManager::Start();

			for (ISystem* system : Registry::Get().GetSystems())
			{
				system->Start();
			}

			glm::vec3& position = Registry::Get().GetComponent<Component::Transform>(m_Scenes[m_Current]->GetPlayer()).position;
			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			GlobalSystems::shadowSystem.Start(color, position);
		}

		void Restart_Systems() 
		{
			Globals::Camera = m_Scenes[m_Current]->GetCamera();

			for (ISystem* system : Registry::Get().GetSystems())
			{
				system->Start();
			}

			glm::vec3& position = Registry::Get().GetComponent<Component::Transform>(m_Scenes[m_Current]->GetPlayer()).position;
			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			GlobalSystems::shadowSystem.Start(color, position);
		}

		void Update_Fixed_Systems(float dt)
		{
			// - Only update at specified frames / s
			while (m_FixedDeltaTime >= 1.0f)
			{
				// Update fixed systems
				for (ISystem* system : Registry::Get().GetSystems())
				{
					system->Run(dt);
				}

				glm::vec3& position = Registry::Get().GetComponent<Component::Transform>(m_Scenes[m_Current]->GetPlayer()).position;
				GlobalSystems::shadowSystem.Run(position);

				m_FixedUpdates++;
				m_FixedDeltaTime--;
			}
		}

		void Update_Systems(float dt)
		{
			if (!m_FixedTimeStep)
			{
				for (ISystem* system : Registry::Get().GetSystems())
				{
					system->Run(dt);
				}

				glm::vec3& position = Registry::Get().GetComponent<Component::Transform>(m_Scenes[m_Current]->GetPlayer()).position;
				GlobalSystems::shadowSystem.Run(position);
			}
			else 
			{
				Update_Fixed_Systems(m_LimitFPS);
			}
		}

		void Clear() 
		{
			for (ISystem* system : Registry::Get().GetSystems())
			{
				system->Clear();
				system->Clean();
			}

			Registry::Get().GetArray<Component::Shadow>().clear();
			Renderer::Get().Clear();

			Registry::Get().Flush();
		}

		void Shutdown() 
		{
			SoundManager::Clean();
			Renderer::Get().Clear();
			GlobalSystems::scriptingSystem.Clear();
			GlobalSystems::spriteRendererSystem.Clear();
			GlobalSystems::windowSystem.Terminate();
		}

		float m_LastTime = 0.0f;
		float m_Timer = m_LastTime;
		float m_DeltaTime = 0.0f;
		int m_Frames = 0, m_Updates = 0;

		float m_LimitFPS = 1.0f / 60.0f;
		float m_FixedDeltaTime = 0.0f;
		int m_FixedUpdates = 0;
	};

}