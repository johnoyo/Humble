#pragma once

#include "GlobalsHeader.h"
#include "..\ecs\Scene.h"

namespace HBL {

	class Level1 : public Scene
	{
		// This is so the class Level1 also inherits the constructor of class Scene
		using Scene::Scene;

	public:
		void Enroll_Entities() override
		{
			ENROLL_ENTITY(background);
			ENROLL_ENTITY(player);
			ENROLL_ENTITY(enemy);
			ENROLL_ENTITY(camera);
			ENROLL_ENTITY(sps);
			ENROLL_ENTITY(lvlHandler);

			for (uint32_t i = 0; i < 400; i++)
				ENROLL_ENTITY(wall[i]);

			for (uint32_t i = 0; i < 100; i++)
				ENROLL_ENTITY(level[i]);
		}

		void Add_Components() override
		{
			ADD_COMPONENT(Transform, background);
			ADD_COMPONENT(Material, background);

			ADD_COMPONENT(Transform, player);
			ADD_COMPONENT(Health, player);
			ADD_COMPONENT(Script, player);
			ADD_COMPONENT(Material, player);
			ADD_COMPONENT(CollisionBox, player);
			//ADD_COMPONENT(Gravity, player);

			ADD_COMPONENT(Transform, enemy);
			ADD_COMPONENT(Script, enemy);
			ADD_COMPONENT(Material, enemy);
			ADD_COMPONENT(CollisionBox, enemy);
			ADD_COMPONENT(Gravity, enemy);
			ADD_COMPONENT(Shadow, enemy);

			ADD_COMPONENT(Transform, camera);

			ADD_COMPONENT(Material, sps);

			ADD_COMPONENT(Script, lvlHandler);

			for (uint32_t i = 0; i < 400; i++) {
				ADD_COMPONENT(Transform, wall[i]);
				ADD_COMPONENT(CollisionBox, wall[i]);
				ADD_COMPONENT(Material, wall[i]);
				ADD_COMPONENT(Shadow, wall[i]);
			}

			for (uint32_t i = 0; i < 100; i++) {
				ADD_COMPONENT(Transform, level[i]);
				ADD_COMPONENT(CollisionBox, level[i]);
				ADD_COMPONENT(Material, level[i]);
			}
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

			for (uint32_t i = 0; i < 100; i++)
				GET_COMPONENT(Transform, level[i]).Static = true;
		}

	};

}