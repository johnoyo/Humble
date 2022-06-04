#pragma once
#include <vector>
#include "Header.h"
#include "Utilities.h"
#include "Scene.h"
#include "../Application/SystemsHeader.h"
#include "Managers/LevelManager.h"

namespace HBL {

	class Application {
	public:

		void Add_Scene(Scene* scene);

		void Load_New_Scene(int level)
		{
			if (level != current) {
				current = level;
				scriptingSystem.Start(current);
			}
		}

		void Start() {
			scenes[current]->Enroll();
			scenes[current]->Add();
			scenes[current]->Init();

			Initialize();

			while (!windowSystem.Window_Should_Close()) {
				// - Measure time
				nowTime = glfwGetTime();
				deltaTime += (nowTime - lastTime) / limitFPS;
				lastTime = nowTime;

				// - Only update at 60 frames / s
				while (deltaTime >= 1.0) {

					Load_New_Scene(LevelManager::GetCurrentLevel());
					scenes[current]->Update();

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

		void Initialize() {
			windowSystem.Start(0);
			textureSystem.Start();
			renderingSystem.Start(cameraSystem.Get_View_Projection_Matrix());
			scriptingSystem.Start(LevelManager::GetCurrentLevel());
			collisionSystem.Start();
			gravitySystem.Start(6.0f, -6.0f);
			cameraSystem.Start();
			transformSystem.Start();
			shadowSystem.Start(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), GET_COMPONENT(Transform, player).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
			SoundManager::Start();
		}

		void Shutdown() {
			renderingSystem.Clear();
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