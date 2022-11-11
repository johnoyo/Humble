#pragma once

#include "..\Scripts\PlayerScript.h"
#include "..\Scripts\EnemyScript.h"
#include "..\Scripts\LevelHandlerScript.h"

namespace HBL {

	class Level0 final : public IScene
	{
		// This is so the class Level0 also inherits the constructor of class Scene
		using IScene::IScene;

	public:
		void Enroll_Entities() override
		{
			HBL::Random::Init();

			std::cout << "Random Float: " << Random::Float(0.0f, 10.0f) << "\n";
			std::cout << "Random Double: " << Random::Double(0.0, 10.0) << "\n";
			std::cout << "Random Int: " << Random::Int32(-20, 5) << "\n";

			Globals::s_Registry.EnrollEntity(background);
			Globals::s_Registry.EnrollEntity(player);
			Globals::s_Registry.EnrollEntity(enemy);
			Globals::s_Registry.EnrollEntity(camera);
			Globals::s_Registry.EnrollEntity(lvlHandler);
			Globals::s_Registry.EnrollEntity(sps);
			Globals::s_Registry.EnrollEntity(text);
			Globals::s_Registry.EnrollEntity(FPSCounter);
			
			for (uint32_t i = 0; i < 400; i++)
				Globals::s_Registry.EnrollEntity(wall[i]);
			
			for (uint32_t i = 0; i < 10000; i++)
				Globals::s_Registry.EnrollEntity(level[i]);
		}

		void Add_Components() override
		{
			Globals::s_Registry.AddComponent<Component::Transform>(background);
			Globals::s_Registry.AddComponent<Component::SpriteRenderer>(background);
			
			Globals::s_Registry.AddComponent<Component::Transform>(player);
			Globals::s_Registry.AddComponent<Component::Script>(player);
			Globals::s_Registry.AddComponent<Component::SpriteRenderer>(player);
			Globals::s_Registry.AddComponent<Component::Animation>(player);
			Globals::s_Registry.AddComponent<Component::CollisionBox>(player);
			Globals::s_Registry.AddComponent<Component::Health>(player);
			
			Globals::s_Registry.AddComponent<Component::Transform>(enemy);
			Globals::s_Registry.AddComponent<Component::Script>(enemy);
			Globals::s_Registry.AddComponent<Component::SpriteRenderer>(enemy);
			Globals::s_Registry.AddComponent<Component::CollisionBox>(enemy);
			Globals::s_Registry.AddComponent<Component::Gravity>(enemy);
			Globals::s_Registry.AddComponent<Component::Shadow>(enemy);
			Globals::s_Registry.AddComponent<Component::Health>(enemy);
			
			Globals::s_Registry.AddComponent<Component::Script>(lvlHandler);
				
			Globals::s_Registry.AddComponent<Component::Transform>(camera);
			
			Globals::s_Registry.AddComponent<Component::SpriteRenderer>(sps);
				
			for (uint32_t i = 0; i < 400; i++) 
			{
				Globals::s_Registry.AddComponent<Component::Transform>(wall[i]);
				Globals::s_Registry.AddComponent<Component::CollisionBox>(wall[i]);
				Globals::s_Registry.AddComponent<Component::SpriteRenderer>(wall[i]);
				Globals::s_Registry.AddComponent<Component::Shadow>(wall[i]);
			}
			
			for (uint32_t i = 0; i < 10000; i++) 
			{
				Globals::s_Registry.AddComponent<Component::Transform>(level[i]);
				Globals::s_Registry.AddComponent<Component::CollisionBox>(level[i]);
				Globals::s_Registry.AddComponent<Component::SpriteRenderer>(level[i]);
			}

			Globals::s_Registry.AddComponent<Component::TextTransform>(text);
			Globals::s_Registry.AddComponent<Component::Text>(text);

			Globals::s_Registry.AddComponent<Component::TextTransform>(FPSCounter);
			Globals::s_Registry.AddComponent<Component::Text>(FPSCounter);
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
			
			for (uint32_t i = 0; i < 10000; i++)
			{
				Globals::s_Registry.GetComponent<Component::Transform>(level[i]).Static = true;
				Globals::s_Registry.GetComponent<Component::Transform>(level[i]).position.x = Random::Float(0.0f, 800.0f);
				Globals::s_Registry.GetComponent<Component::Transform>(level[i]).position.y = Random::Float(0.0f, 800.0f);
			}

			Globals::s_Registry.GetComponent<Component::TextTransform>(text).scale.x = 2.0f;
			Globals::s_Registry.GetComponent<Component::TextTransform>(text).scale.y = 2.0f;
			Globals::s_Registry.GetComponent<Component::Text>(text).text = "aaaaaaaaaaaaaaaaaaaaaaaaaaa";

			Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).position.x = 500.0f;
			Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).position.y = 500.0f;

			Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).sreenSpaceOffset.x = 100.0f;
			Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).sreenSpaceOffset.y = 600.0f;

			Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).scale.x = 1.0f;
			Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).scale.y = 1.0f;

			Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).screenSpace = true;
			Globals::s_Registry.GetComponent<Component::Text>(FPSCounter).text = "";

			Globals::s_Registry.GetComponent<Component::Health>(player).health = 99;
			Globals::s_Registry.GetComponent<Component::Health>(player).health = 88;
		}

		void Init_Systems() override
		{
			GlobalSystems::cameraSystem.Initialize(0.0f, GlobalSystems::windowSystem.Get_Width(), 0.0f, GlobalSystems::windowSystem.Get_Height());
			GlobalSystems::gravitySystem.InitializeGravity(1000.0f, -1000.0f);
		}

	};

}

