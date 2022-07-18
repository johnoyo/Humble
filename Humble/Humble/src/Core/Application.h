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
		Application(float width, float height, const std::string& name, bool full_screen);

		void Add_Scene(IScene* scene);

		void Register_System(ISystem* system);

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

			scenes[current]->Init_Systems();

			scenes[current]->Enroll_Entities();
			scenes[current]->Add_Components();
			scenes[current]->Init_Components();

			// Initialize Systems
			Initialize();

			while (!GlobalSystems::windowSystem.Window_Should_Close()) {
				// - Measure time
				nowTime = glfwGetTime();
				deltaTime += (nowTime - lastTime) / limitFPS;
				lastTime = nowTime;

				// - Only update at 60 frames / s
				while (deltaTime >= 1.0) {

					Manage_Scenes();

					// Update Systems
					Update();

					updates++;
					deltaTime--;
				}

				// - Render at maximum possible frames
				GlobalSystems::renderingSystem.Render(GlobalSystems::cameraSystem.Get_View_Projection_Matrix());
				frames++;

				// - Reset after one second
				if (glfwGetTime() - timer > 1.0) {
					timer++;
					ENGINE_LOG("FPS: %d Updates: %d", frames, updates);
					updates = 0, frames = 0;
				}

				GlobalSystems::windowSystem.Swap_Buffers();
				GlobalSystems::windowSystem.Poll_For_Events();
			}

			Shutdown();
		}

	private:
		std::vector<IScene*> scenes;
		std::vector<ISystem*> systems;

		uint32_t current = 0;

		void Register_Systems()
		{
			Register_System(&GlobalSystems::materialSystem);
			Register_System(&GlobalSystems::animationSystem);
			Register_System(&GlobalSystems::scriptingSystem);
			Register_System(&GlobalSystems::transformSystem);
			Register_System(&GlobalSystems::collisionSystem);
			Register_System(&GlobalSystems::gravitySystem);
		}

		void Initialize() 
		{
			GlobalSystems::windowSystem.Create(0);
			GlobalSystems::cameraSystem.Create();
			GlobalSystems::renderingSystem.Initialize(GlobalSystems::cameraSystem.Get_View_Projection_Matrix());

			SoundManager::Start();

			for (ISystem* system : systems)
			{
				system->Start();
			}

			glm::vec3& position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			GlobalSystems::shadowSystem.Start(color, position);
		}

		void Restart_Systems() 
		{
			GlobalSystems::cameraSystem.Create();
			GlobalSystems::renderingSystem.Initialize(GlobalSystems::cameraSystem.Get_View_Projection_Matrix());

			for (ISystem* system : systems)
			{
				system->Start();
			}

			glm::vec3& position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
			GlobalSystems::shadowSystem.Start(color, position);
		}

		void Update()
		{
			for (ISystem* system : systems)
			{
				system->Run();
			}

			glm::vec3& position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			GlobalSystems::shadowSystem.Run(position);
		}

		void Clear() 
		{
			for (ISystem* system : systems)
			{
				system->Clear();
			}

			GlobalSystems::shadowSystem.Clear();
			GlobalSystems::renderingSystem.Clear();

			Globals::ecs.Flush(Globals::entities);
		}

		void Shutdown() 
		{
			GlobalSystems::renderingSystem.Clear();
			GlobalSystems::scriptingSystem.Clear();
			GlobalSystems::materialSystem.Clear();
			GlobalSystems::windowSystem.Terminate();
		}

		double limitFPS = 1.0 / 60.0;
		double lastTime = glfwGetTime();
		double timer = lastTime;
		double deltaTime = 0, nowTime = 0;
		int frames = 0, updates = 0;

	};

}