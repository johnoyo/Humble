#pragma once

#include "..\Scripts\PlayerScript.h"
#include "..\Scripts\EnemyScript.h"
#include "..\Scripts\LevelHandlerScript.h"

namespace HBL 
{
	class Level1 final : public IScene
	{
		// This is so the class Level1 also inherits the constructor of class Scene
		using IScene::IScene;

	public:
		void EnrollEntities() override
		{
			Registry::Get().EnrollEntity(background);
			Registry::Get().EnrollEntity(player);
			Registry::Get().EnrollEntity(enemy);
			Registry::Get().EnrollEntity(camera, "Camera");
			Registry::Get().EnrollEntity(sps);
			Registry::Get().EnrollEntity(lvlHandler);

			for (uint32_t i = 0; i < 400; i++)
				Registry::Get().EnrollEntity(wall[i]);

			for (uint32_t i = 0; i < 100; i++)
				Registry::Get().EnrollEntity(level[i]);
		}

		void AddComponents() override
		{
			Registry::Get().AddComponent<Component::Transform>(background);
			Registry::Get().AddComponent<Component::SpriteRenderer>(background);

			Registry::Get().AddComponent<Component::Transform>(player);
			Registry::Get().AddComponent<Component::Script>(player);
			Registry::Get().AddComponent<Component::SpriteRenderer>(player);
			Registry::Get().AddComponent<Component::CollisionBox>(player);
			Registry::Get().AddComponent<Component::Animation>(player);

			Registry::Get().AddComponent<Component::Transform>(enemy);
			Registry::Get().AddComponent<Component::Script>(enemy);
			Registry::Get().AddComponent<Component::SpriteRenderer>(enemy);
			Registry::Get().AddComponent<Component::CollisionBox>(enemy);
			Registry::Get().AddComponent<Component::Gravity>(enemy);
			Registry::Get().AddComponent<Component::Shadow>(enemy);

			Registry::Get().AddComponent<Component::Camera>(camera);
			Registry::Get().AddComponent<Component::Transform>(camera);

			Registry::Get().AddComponent<Component::SpriteRenderer>(sps);

			Registry::Get().AddComponent<Component::Script>(lvlHandler);

			for (uint32_t i = 0; i < 400; i++) 
			{
				Registry::Get().AddComponent<Component::Transform>(wall[i]);
				Registry::Get().AddComponent<Component::CollisionBox>(wall[i]);
				Registry::Get().AddComponent<Component::SpriteRenderer>(wall[i]);
				Registry::Get().AddComponent<Component::Shadow>(wall[i]);
			}

			for (uint32_t i = 0; i < 100; i++) 
			{
				Registry::Get().AddComponent<Component::Transform>(level[i]);
				Registry::Get().AddComponent<Component::CollisionBox>(level[i]);
				Registry::Get().AddComponent<Component::SpriteRenderer>(level[i]);
			}
		}

		void InitComponents() override
		{
			Registry::Get().GetComponent<Component::Script>(player).script.push_back(new PlayerScript());
			Registry::Get().GetComponent<Component::Script>(enemy).script.push_back(new EnemyScript());
			Registry::Get().GetComponent<Component::Script>(lvlHandler).script.push_back(new LevelHandlerScript());

			Registry::Get().GetComponent<Component::Transform>(player).Static = false;
			Registry::Get().GetComponent<Component::Transform>(enemy).Static = false;
			Registry::Get().GetComponent<Component::Transform>(background).Static = false;

			Registry::Get().GetComponent<Component::Shadow>(enemy).source = &player;
			Registry::Get().GetComponent<Component::Gravity>(enemy).force = 1000.0f;
			Registry::Get().GetComponent<Component::Gravity>(enemy).threshold = -1000.0f;

			Registry::Get().GetComponent<Component::Camera>(camera).projection = glm::ortho(
																				0.0f, Systems::Window.GetWidth(), 
																				0.0f, Systems::Window.GetHeight(), 
																				-1.0f, 1.0f);

			for (uint32_t i = 0; i < 400; i++)
			{
				Registry::Get().GetComponent<Component::Transform>(wall[i]).Static = true;
				Registry::Get().GetComponent<Component::Shadow>(wall[i]).source = &player;
			}

			for (uint32_t i = 0; i < 100; i++)
				Registry::Get().GetComponent<Component::Transform>(level[i]).Static = true;
		}

	};
}