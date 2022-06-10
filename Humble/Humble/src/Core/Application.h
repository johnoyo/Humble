#pragma once
#include <vector>
#include "Header.h"
#include "SystemsHeader.h"
#include "Utilities.h"
#include "Scene.h"

namespace HBL {

	class Application {
	public:

		void Add_Scene(Scene* scene);

		void Manage_Scenes()
		{
			if (Scene_Change) {
				Scene_Change = false;
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

			while (!windowSystem.Window_Should_Close()) {
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
				renderingSystem.Render(cameraSystem.Get_View_Projection_Matrix());
				frames++;

				// - Reset after one second
				if (glfwGetTime() - timer > 1.0) {
					timer++;
					ENGINE_LOG("FPS: %d Updates: %d", frames, updates);
					updates = 0, frames = 0;
				}

				windowSystem.Swap_Buffers();
				windowSystem.Poll_For_Events();
			}

			Shutdown();
		}

	private:
		std::vector<Scene*> scenes;
		uint32_t current = 0;

		void Initialize() 
		{
			windowSystem.Start(0);
			cameraSystem.Start();
			SoundManager::Start();

			renderingSystem.Start(cameraSystem.Get_View_Projection_Matrix());
			textureSystem.Start();
			scriptingSystem.Start(current);
			collisionSystem.Start();
			gravitySystem.Start(6.0f, -6.0f);
			transformSystem.Start();

			shadowSystem.Start(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), GET_COMPONENT(Transform, scenes[current]->Get_Player()).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
		}

		void Restart_Systems() 
		{
			renderingSystem.Start(cameraSystem.Get_View_Projection_Matrix());
			cameraSystem.Start();
			textureSystem.Start();
			scriptingSystem.Start(current);
			collisionSystem.Start();
			gravitySystem.Start(6.0f, -6.0f);
			transformSystem.Start();

			shadowSystem.Start(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), GET_COMPONENT(Transform, scenes[current]->Get_Player()).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
		}

		void Update()
		{
			scriptingSystem.Run(current);
			textureSystem.Run(renderingSystem.Get_Vertex_Buffer());
			gravitySystem.Run();
			transformSystem.Run(renderingSystem.Get_Vertex_Buffer());
			collisionSystem.Run(renderingSystem.Get_Vertex_Buffer());
			shadowSystem.Run(GET_COMPONENT(Transform, scenes[current]->Get_Player()).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
		}

		void Clear() 
		{
			scriptingSystem.Clear(current);
			textureSystem.Clear();
			gravitySystem.Clear();
			transformSystem.Clear();
			collisionSystem.Clear();
			shadowSystem.Clear();
			renderingSystem.Clear();

			ecs.Flush(entities);
		}

		void Shutdown() 
		{
			renderingSystem.Clear();
			scriptingSystem.Clear(current);
			textureSystem.Clear();
			windowSystem.Terminate();
		}

		double limitFPS = 1.0 / 60.0;
		double lastTime = glfwGetTime();
		double timer = lastTime;
		double deltaTime = 0, nowTime = 0;
		int frames = 0, updates = 0;

	};

}