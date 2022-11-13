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

			Registry::Get().EnrollEntity(background);
			Registry::Get().EnrollEntity(player);
			Registry::Get().EnrollEntity(enemy);
			Registry::Get().EnrollEntity(camera, "Camera");
			Registry::Get().EnrollEntity(lvlHandler);
			Registry::Get().EnrollEntity(sps);
			Registry::Get().EnrollEntity(text);
			Registry::Get().EnrollEntity(FPSCounter);
			
			for (uint32_t i = 0; i < 400; i++)
				Registry::Get().EnrollEntity(wall[i]);
			
			for (uint32_t i = 0; i < 10000; i++)
				Registry::Get().EnrollEntity(level[i]);
		}

		void Add_Components() override
		{
			Registry::Get().AddComponent<Component::Transform>(background);
			Registry::Get().AddComponent<Component::SpriteRenderer>(background);
			
			Registry::Get().AddComponent<Component::Transform>(player);
			Registry::Get().AddComponent<Component::Script>(player);
			Registry::Get().AddComponent<Component::SpriteRenderer>(player);
			Registry::Get().AddComponent<Component::Animation>(player);
			Registry::Get().AddComponent<Component::CollisionBox>(player);
			Registry::Get().AddComponent<Component::Health>(player);
			
			Registry::Get().AddComponent<Component::Transform>(enemy);
			Registry::Get().AddComponent<Component::Script>(enemy);
			Registry::Get().AddComponent<Component::SpriteRenderer>(enemy);
			Registry::Get().AddComponent<Component::CollisionBox>(enemy);
			Registry::Get().AddComponent<Component::Gravity>(enemy);
			Registry::Get().AddComponent<Component::Shadow>(enemy);
			Registry::Get().AddComponent<Component::Health>(enemy);
			
			Registry::Get().AddComponent<Component::Script>(lvlHandler);
				
			Registry::Get().AddComponent<Component::Transform>(camera);
			
			Registry::Get().AddComponent<Component::SpriteRenderer>(sps);
				
			for (uint32_t i = 0; i < 400; i++) 
			{
				Registry::Get().AddComponent<Component::Transform>(wall[i]);
				Registry::Get().AddComponent<Component::CollisionBox>(wall[i]);
				Registry::Get().AddComponent<Component::SpriteRenderer>(wall[i]);
				Registry::Get().AddComponent<Component::Shadow>(wall[i]);
			}
			
			for (uint32_t i = 0; i < 10000; i++) 
			{
				Registry::Get().AddComponent<Component::Transform>(level[i]);
				Registry::Get().AddComponent<Component::CollisionBox>(level[i]);
				Registry::Get().AddComponent<Component::SpriteRenderer>(level[i]);
			}

			Registry::Get().AddComponent<Component::TextTransform>(text);
			Registry::Get().AddComponent<Component::Text>(text);

			Registry::Get().AddComponent<Component::TextTransform>(FPSCounter);
			Registry::Get().AddComponent<Component::Text>(FPSCounter);
		}

		void Init_Components() override
		{
			Registry::Get().GetComponent<Component::Script>(player).script.push_back(new PlayerScript());
			Registry::Get().GetComponent<Component::Script>(enemy).script.push_back(new EnemyScript());
			Registry::Get().GetComponent<Component::Script>(lvlHandler).script.push_back(new LevelHandlerScript());

			Registry::Get().GetComponent<Component::Transform>(player).Static = false;
			Registry::Get().GetComponent<Component::Transform>(enemy).Static = false;
			Registry::Get().GetComponent<Component::Transform>(background).Static = false;
			
			for (uint32_t i = 0; i < 400; i++)
				Registry::Get().GetComponent<Component::Transform>(wall[i]).Static = true;
			
			for (uint32_t i = 0; i < 10000; i++)
			{
				Registry::Get().GetComponent<Component::Transform>(level[i]).Static = true;
				Registry::Get().GetComponent<Component::Transform>(level[i]).position.x = Random::Float(0.0f, 800.0f);
				Registry::Get().GetComponent<Component::Transform>(level[i]).position.y = Random::Float(0.0f, 800.0f);
			}

			Registry::Get().GetComponent<Component::TextTransform>(text).scale.x = 2.0f;
			Registry::Get().GetComponent<Component::TextTransform>(text).scale.y = 2.0f;
			Registry::Get().GetComponent<Component::Text>(text).text = "aaaaaaaaaaaaaaaaaaaaaaaaaaa";

			Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).position.x = 500.0f;
			Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).position.y = 500.0f;

			Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).sreenSpaceOffset.x = 100.0f;
			Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).sreenSpaceOffset.y = 600.0f;

			Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).scale.x = 1.0f;
			Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).scale.y = 1.0f;

			Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).screenSpace = true;
			Registry::Get().GetComponent<Component::Text>(FPSCounter).text = "";

			Registry::Get().GetComponent<Component::Health>(player).health = 99;
			Registry::Get().GetComponent<Component::Health>(player).health = 88;
		}

		void Init_Systems() override
		{
			GlobalSystems::cameraSystem.Initialize(0.0f, GlobalSystems::windowSystem.Get_Width(), 0.0f, GlobalSystems::windowSystem.Get_Height());
			GlobalSystems::gravitySystem.InitializeGravity(1000.0f, -1000.0f);
		}

	};

}

