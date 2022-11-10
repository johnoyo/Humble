#pragma once

#include "..\Scripts\PlayerScript.h"
#include "..\Scripts\EnemyScript.h"
#include "..\Scripts\LevelHandlerScript.h"

namespace HBL {

	class Level1 final : public IScene
	{
		// This is so the class Level1 also inherits the constructor of class Scene
		using IScene::IScene;

	public:
		void Enroll_Entities() override
		{
			Globals::s_Registry.EnrollEntity(background);
			Globals::s_Registry.EnrollEntity(player);
			Globals::s_Registry.EnrollEntity(enemy);
			Globals::s_Registry.EnrollEntity(camera);
			Globals::s_Registry.EnrollEntity(sps);
			Globals::s_Registry.EnrollEntity(lvlHandler);

			for (uint32_t i = 0; i < 400; i++)
				Globals::s_Registry.EnrollEntity(wall[i]);

			for (uint32_t i = 0; i < 100; i++)
				Globals::s_Registry.EnrollEntity(level[i]);
		}

		void Add_Components() override
		{
			Globals::s_Registry.AddComponent<Component::Transform>(background);
			Globals::s_Registry.AddComponent<Component::Material>(background);

			Globals::s_Registry.AddComponent<Component::Transform>(player);
			Globals::s_Registry.AddComponent<Component::Script>(player);
			Globals::s_Registry.AddComponent<Component::Material>(player);
			Globals::s_Registry.AddComponent<Component::CollisionBox>(player);
			Globals::s_Registry.AddComponent<Component::Animation>(player);

			Globals::s_Registry.AddComponent<Component::Transform>(enemy);
			Globals::s_Registry.AddComponent<Component::Script>(enemy);
			Globals::s_Registry.AddComponent<Component::Material>(enemy);
			Globals::s_Registry.AddComponent<Component::CollisionBox>(enemy);
			Globals::s_Registry.AddComponent<Component::Gravity>(enemy);
			Globals::s_Registry.AddComponent<Component::Shadow>(enemy);

			Globals::s_Registry.AddComponent<Component::Transform>(camera);

			Globals::s_Registry.AddComponent<Component::Material>(sps);

			Globals::s_Registry.AddComponent<Component::Script>(lvlHandler);

			for (uint32_t i = 0; i < 400; i++) 
			{
				Globals::s_Registry.AddComponent<Component::Transform>(wall[i]);
				Globals::s_Registry.AddComponent<Component::CollisionBox>(wall[i]);
				Globals::s_Registry.AddComponent<Component::Material>(wall[i]);
				Globals::s_Registry.AddComponent<Component::Shadow>(wall[i]);
			}

			for (uint32_t i = 0; i < 100; i++) 
			{
				Globals::s_Registry.AddComponent<Component::Transform>(level[i]);
				Globals::s_Registry.AddComponent<Component::CollisionBox>(level[i]);
				Globals::s_Registry.AddComponent<Component::Material>(level[i]);
			}
		}

		void Init_Components() override
		{
			Globals::s_Registry.GetComponent<Component::Script>(player).script.push_back(new PlayerScript());
			Globals::s_Registry.GetComponent<Component::Script>(enemy).script.push_back(new EnemyScript());
			Globals::s_Registry.GetComponent<Component::Script>(lvlHandler).script.push_back(new LevelHandlerScript());

			Globals::s_Registry.GetComponent<Component::Transform>(player).Static = false;
			Globals::s_Registry.GetComponent<Component::Transform>(enemy).Static = false;
			Globals::s_Registry.GetComponent<Component::Transform>(background).Static = false;

			for (uint32_t i = 0; i < 400; i++)
				Globals::s_Registry.GetComponent<Component::Transform>(wall[i]).Static = true;

			for (uint32_t i = 0; i < 100; i++)
				Globals::s_Registry.GetComponent<Component::Transform>(level[i]).Static = true;
		}

		void Init_Systems() override
		{
			GlobalSystems::cameraSystem.Initialize(0.0f, GlobalSystems::windowSystem.Get_Width(), 0.0f, GlobalSystems::windowSystem.Get_Height());
			GlobalSystems::gravitySystem.InitializeGravity(1000.0f, -1000.0f);
		}

	};

}