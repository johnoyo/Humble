#pragma once
#include <vector>
#include "Header.h"
#include "SystemsHeader.h"
#include "Utilities.h"
#include "HumbleAPI.h"
#include "Scene.h"

namespace HBL {

	class HBL_API Application {
	public:

		void Add_Scene(Scene* scene);

		void Manage_Scenes()
		{
			if (Globals::Scene_Change) {
				Globals::Scene_Change = false;
				current++;

				// Clear Systems and ECS
				Clear();

				// Initialize Systems
				scenes[current]->Enroll_Entities();
				scenes[current]->Add_Components();
				scenes[current]->Init_Components();

				// Initialize Systems
				Restart_Systems();
			}
		}

		void Start() {
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
		std::vector<Scene*> scenes;
		uint32_t current = 0;

		void Initialize() 
		{
			GlobalSystems::windowSystem.Start(0);
			GlobalSystems::cameraSystem.Start();
			SoundManager::Start();

			GlobalSystems::renderingSystem.Start(GlobalSystems::cameraSystem.Get_View_Projection_Matrix());
			GlobalSystems::textureSystem.Start();
			GlobalSystems::scriptingSystem.Start(current);
			GlobalSystems::collisionSystem.Start();
			GlobalSystems::gravitySystem.Start(6.0f, -6.0f);
			GlobalSystems::transformSystem.Start();

			glm::vec3& position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;

			GlobalSystems::shadowSystem.Start(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), position, GlobalSystems::renderingSystem.Get_Vertex_Buffer(), GlobalSystems::renderingSystem);
		}

		void Restart_Systems() 
		{
			GlobalSystems::renderingSystem.Start(GlobalSystems::cameraSystem.Get_View_Projection_Matrix());
			GlobalSystems::cameraSystem.Start();
			GlobalSystems::textureSystem.Start();
			GlobalSystems::scriptingSystem.Start(current);
			GlobalSystems::collisionSystem.Start();
			GlobalSystems::gravitySystem.Start(6.0f, -6.0f);
			GlobalSystems::transformSystem.Start();

			glm::vec3 position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			GlobalSystems::shadowSystem.Start(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), position, GlobalSystems::renderingSystem.Get_Vertex_Buffer(), GlobalSystems::renderingSystem);
		}

		void Update()
		{
			GlobalSystems::scriptingSystem.Run(current);
			GlobalSystems::textureSystem.Run(GlobalSystems::renderingSystem.Get_Vertex_Buffer());
			GlobalSystems::transformSystem.Run(GlobalSystems::renderingSystem.Get_Vertex_Buffer());
			GlobalSystems::collisionSystem.Run(GlobalSystems::renderingSystem.Get_Vertex_Buffer());
			GlobalSystems::gravitySystem.Run();

			glm::vec3 position = GET_COMPONENT(Transform, scenes[current]->Get_Player()).position;
			GlobalSystems::shadowSystem.Run(position, GlobalSystems::renderingSystem.Get_Vertex_Buffer(), GlobalSystems::renderingSystem);
		}

		void Clear() 
		{
			GlobalSystems::scriptingSystem.Clear(current);
			GlobalSystems::textureSystem.Clear();
			GlobalSystems::gravitySystem.Clear();
			GlobalSystems::transformSystem.Clear();
			GlobalSystems::collisionSystem.Clear();
			GlobalSystems::shadowSystem.Clear();
			GlobalSystems::renderingSystem.Clear();

			Globals::ecs.Flush(Globals::entities);
		}

		void Shutdown() 
		{
			GlobalSystems::renderingSystem.Clear();
			GlobalSystems::scriptingSystem.Clear(current);
			GlobalSystems::textureSystem.Clear();
			GlobalSystems::windowSystem.Terminate();
		}

		double limitFPS = 1.0 / 60.0;
		double lastTime = glfwGetTime();
		double timer = lastTime;
		double deltaTime = 0, nowTime = 0;
		int frames = 0, updates = 0;

	};

}