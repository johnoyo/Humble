#pragma once
#include <vector>
#include "Header.h"
#include "SystemsHeader.h"
#include "Utilities.h"
#include "HumbleAPI.h"
#include "IScene.h"
#include "ISystem.h"

namespace HBL {

	class HBL_API Application {
	public:
		Application(float width, float height, const std::string& name, bool full_screen, bool vSync);

		void Add_Scene(IScene* scene);

		void Register_System(ISystem* system);

		void Register_Physics_System(ISystem* system);

		void Manage_Scenes()
		{
			if (Globals::Scene_Change) {
				if (current < scenes.size() - 1)
				{
					Globals::Scene_Change = false;
					Globals::Current_Level++;
					current++;

					// Clear Systems and ECS
					Clear();

					// Initialize Systems
					scenes[current]->Init_Systems();

					scenes[current]->Enroll_Entities();
					scenes[current]->Add_Components();
					scenes[current]->Init_Components();

					// Initialize Systems
					Restart_Systems();
				}
			}
		}

		void Start() {

			Register_Systems();
			Register_Physics_Systems();

			scenes[current]->Init_Systems();

			scenes[current]->Enroll_Entities();
			scenes[current]->Add_Components();
			scenes[current]->Init_Components();

			// Initialize Systems
			Initialize_Systems();

			while (!GlobalSystems::windowSystem.Window_Should_Close()) 
			{
				// Measure time and delta time
				float time = (float)glfwGetTime();
				deltaTime = time - lastTime;
				lastTime = time;

				// Manage scenes and level switching 
				Manage_Scenes();

				// Update Registered Systems
				Update_Systems(deltaTime);

				// Update Registered Physics Systems
				Update_Physics_Systems(deltaTime);

				// Update collision system
				Update_Collision_System(deltaTime);

				// Render
				Renderer::Get().Render(GlobalSystems::cameraSystem.Get_View_Projection_Matrix());
				frames++;

				// Reset after one second
				if (glfwGetTime() - timer > 1.0) 
				{
					// Display frame rate at window bar
					std::stringstream ss;
					ss << GlobalSystems::windowSystem.Get_Title() << " [" << frames << " FPS]";
					glfwSetWindowTitle(GlobalSystems::windowSystem.Get_Window(), ss.str().c_str());

					// Log framerate and delta time to console
					ENGINE_LOG("FPS: %d DeltaTime: %f", frames, deltaTime);

					timer++;
					frames = 0;
					fixedUpdates = 0;
				}

				GlobalSystems::windowSystem.Swap_Buffers();
				GlobalSystems::windowSystem.Poll_For_Events();
			}

			Shutdown();
		}

	private:
		std::vector<IScene*> scenes;
		std::vector<ISystem*> systems;
		std::vector<ISystem*> physicsSystems;

		uint32_t current = 0;

		void Register_Systems()
		{
			Register_System(&GlobalSystems::renderingSystem);
			Register_System(&GlobalSystems::materialSystem);
			Register_System(&GlobalSystems::animationSystem);
			Register_System(&GlobalSystems::scriptingSystem);
			Register_System(&GlobalSystems::transformSystem);
			Register_System(&GlobalSystems::textSystem);
		}

		void Register_Physics_Systems()
		{
			Register_Physics_System(&GlobalSystems::gravitySystem);
		}

		void Initialize_Systems() 
		{
			GlobalSystems::windowSystem.Create();
			GlobalSystems::cameraSystem.Create();

			SoundManager::Start();

			for (ISystem* system : systems)
			{
				system->Start();
			}

			GlobalSystems::collisionSystem.Start();

			glm::vec3& position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			GlobalSystems::shadowSystem.Start(color, position);

			for (ISystem* system : physicsSystems)
			{
				system->Start();
			}
		}

		void Restart_Systems() 
		{
			GlobalSystems::cameraSystem.Create();

			for (ISystem* system : systems)
			{
				system->Start();
			}

			GlobalSystems::collisionSystem.Start();

			glm::vec3& position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			GlobalSystems::shadowSystem.Start(color, position);

			for (ISystem* system : physicsSystems)
			{
				system->Start();
			}
		}

		void Update_Physics_Systems(float dt)
		{
			// - Measure fixed time
			fixedNowTime = glfwGetTime();
			fixedDeltaTime += (fixedNowTime - fixedLastTime) / limitFPS;
			fixedLastTime = fixedNowTime;

			// - Only update at 30 frames / s
			while (fixedDeltaTime >= 1.0)
			{
				// Update physics systems
				for (ISystem* system : physicsSystems)
				{
					system->Run(0.033f);
				}

				fixedUpdates++;
				fixedDeltaTime--;
			}
		}

		void Update_Systems(float dt)
		{
			for (ISystem* system : systems)
			{
				system->Run(dt);
			}

			glm::vec3& position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			GlobalSystems::shadowSystem.Run(position);
		}

		void Update_Collision_System(float dt) 
		{
			GlobalSystems::collisionSystem.Run(dt);
		}

		void Clear() 
		{
			for (ISystem* system : systems)
			{
				system->Clear();
			}

			for (ISystem* system : physicsSystems)
			{
				system->Clear();
			}

			GlobalSystems::collisionSystem.Clear();
			GlobalSystems::shadowSystem.Clear();
			Renderer::Get().Clear();

			Globals::ecs.Flush(Globals::entities);
		}

		void Shutdown() 
		{
			Renderer::Get().Clear();
			GlobalSystems::scriptingSystem.Clear();
			GlobalSystems::materialSystem.Clear();
			GlobalSystems::windowSystem.Terminate();
		}

		float lastTime = 0.0f;
		float timer = lastTime;
		float deltaTime = 0.0f, nowTime = 0.0f;
		int frames = 0, updates = 0;

		double limitFPS = 1.0 / 30.0;
		double fixedLastTime = glfwGetTime();
		double fixedTimer = fixedLastTime;
		double fixedDeltaTime = 0, fixedNowTime = 0;
		int fixedUpdates = 0;

	};

}