#pragma once

#include "..\Scripts\PlayerScript.h"
#include "..\Scripts\EnemyScript.h"
#include "..\Scripts\LevelHandlerScript.h"

#include <time.h>

namespace HBL {

	class Level0 final : public IScene
	{
		// This is so the class Level0 also inherits the constructor of class Scene
		using IScene::IScene;

	public:
		void Enroll_Entities() override
		{
			HBL::Random::Init();

			std::cout << "Random Float: " << Random::Float(10.0f) << "\n";
			std::cout << "Random Double: " << Random::Double(10.0) << "\n";
			std::cout << "Random Int: " << Random::Int(10) << "\n";

			ENROLL_ENTITY(background);
			ENROLL_ENTITY(player);
			ENROLL_ENTITY(enemy);
			ENROLL_ENTITY(camera);
			ENROLL_ENTITY(lvlHandler);
			ENROLL_ENTITY(sps);
			ENROLL_ENTITY(text);
			
			for (uint32_t i = 0; i < 400; i++)
				ENROLL_ENTITY(wall[i]);
			
			for (uint32_t i = 0; i < 10000; i++)
				ENROLL_ENTITY(level[i]);
		}

		void Add_Components() override
		{
			ADD_COMPONENT(Transform, background);
			ADD_COMPONENT(Material, background);
			
			ADD_COMPONENT(Transform, player);
			ADD_COMPONENT(Script, player);
			ADD_COMPONENT(Material, player);
			ADD_COMPONENT(Animation, player);
			ADD_COMPONENT(CollisionBox, player);
			ADD_MY_COMPONENT(Health, player);
			
			ADD_COMPONENT(Transform, enemy);
			ADD_COMPONENT(Script, enemy);
			ADD_COMPONENT(Material, enemy);
			ADD_COMPONENT(CollisionBox, enemy);
			ADD_COMPONENT(Gravity, enemy);
			ADD_COMPONENT(Shadow, enemy);
			ADD_MY_COMPONENT(Health, enemy);
			
			ADD_COMPONENT(Script, lvlHandler);
				
			ADD_COMPONENT(Transform, camera);
			
			ADD_COMPONENT(Material, sps);
				
			for (uint32_t i = 0; i < 400; i++) {
				ADD_COMPONENT(Transform, wall[i]);
				ADD_COMPONENT(CollisionBox, wall[i]);
				ADD_COMPONENT(Material, wall[i]);
				ADD_COMPONENT(Shadow, wall[i]);
			}
			
			for (uint32_t i = 0; i < 10000; i++) {
				ADD_COMPONENT(Transform, level[i]);
				ADD_COMPONENT(CollisionBox, level[i]);
				ADD_COMPONENT(Material, level[i]);
			}

			ADD_COMPONENT(TextTransform, text);
			ADD_COMPONENT(Text, text);
		}

		void Init_Components() override
		{
			GET_COMPONENT(Script, player).script.push_back(new PlayerScript());
			GET_COMPONENT(Script, enemy).script.push_back(new EnemyScript());
			GET_COMPONENT(Script, lvlHandler).script.push_back(new LevelHandlerScript());

			GET_COMPONENT(Transform, player).Static = false;
			GET_COMPONENT(Transform, enemy).Static = false;
			GET_COMPONENT(Transform, background).Static = false;
			
			for (uint32_t i = 0; i < 400; i++)
				GET_COMPONENT(Transform, wall[i]).Static = true;

			srand(time(NULL));
			
			for (uint32_t i = 0; i < 10000; i++)
			{
				GET_COMPONENT(Transform, level[i]).Static = true;
				GET_COMPONENT(Transform, level[i]).position.x = rand() % 270;
				GET_COMPONENT(Transform, level[i]).position.y = rand() % 270;
			}

			GET_COMPONENT(TextTransform, text).scale.x = 1.5f;
			GET_COMPONENT(TextTransform, text).scale.y = 1.5f;
			GET_COMPONENT(Text, text).text = "tizllllllllllloghkqp";

			GET_MY_COMPONENT(Health, player).health = 99;
			GET_MY_COMPONENT(Health, enemy).health = 88;
			
		}

		void Init_Systems() override
		{
			GlobalSystems::cameraSystem.Initialize(0.0f, GlobalSystems::windowSystem.Get_Width(), 0.0f, GlobalSystems::windowSystem.Get_Height());
			GlobalSystems::gravitySystem.InitializeGravity(1000.0f, -1000.0f);
		}

	};

}

