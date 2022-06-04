#pragma once

#include "SystemsHeader.h"
#include "..\ecs\Scene.h"

#include "../Scripts/PlayerScript.h"
#include "../Scripts/EnemyScript.h"
#include "../Scripts/LevelHandlerScript.h"

namespace HBL {

	class Level0 : public Scene
	{
	public:
		void Enroll() override 
		{
			ENROLL_ENTITY(background);
			ENROLL_ENTITY(player);
			ENROLL_ENTITY(enemy);
			ENROLL_ENTITY(camera);
			ENROLL_ENTITY(lvlHandler);
			ENROLL_ENTITY(sps);
			
			for (uint32_t i = 0; i < 400; i++)
				ENROLL_ENTITY(wall[i]);
			
			for (uint32_t i = 0; i < 10000; i++)
				ENROLL_ENTITY(level[i]);
		}
		void Add() override  
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
			
			ADD_COMPONENT(Script, lvlHandler);
				
			ADD_COMPONENT(Transform, camera);
			
			ADD_COMPONENT(Material, sps);
				
			for (uint32_t i = 0; i < 400; i++) {
				ADD_COMPONENT(Transform, wall[i]);
				ADD_COMPONENT(CollisionBox, wall[i]);
				ADD_COMPONENT(Material, wall[i]);
				//ADD_COMPONENT(Shadow, wall[i]);
			}
			
			for (uint32_t i = 0; i < 10000; i++) {
				ADD_COMPONENT(Transform, level[i]);
				ADD_COMPONENT(CollisionBox, level[i]);
				ADD_COMPONENT(Material, level[i]);
			}
		}
		void Init() override  
		{

			GET_COMPONENT(Script, player).script.push_back(new PlayerScript());
			GET_COMPONENT(Script, enemy).script.push_back(new EnemyScript());
			GET_COMPONENT(Script, lvlHandler).script.push_back(new LevelHandlerScript());
			
			/*GET_COMPONENT(Script, player).init.push_back(Player::Level_0::init);
			GET_COMPONENT(Script, player).update.push_back(Player::Level_0::update);

			GET_COMPONENT(Script, enemy).init.push_back(Enemy::Level_0::init);
			GET_COMPONENT(Script, enemy).init.push_back(Enemy::Level_1::init);
			GET_COMPONENT(Script, enemy).update.push_back(Enemy::Level_0::update);
			GET_COMPONENT(Script, enemy).update.push_back(Enemy::Level_1::update);
			
			GET_COMPONENT(Script, lvlHandler).init.push_back(LevelHandler::Level_0::init);
			GET_COMPONENT(Script, lvlHandler).init.push_back(LevelHandler::Level_1::init);
			GET_COMPONENT(Script, lvlHandler).update.push_back(LevelHandler::Level_0::update);
			GET_COMPONENT(Script, lvlHandler).update.push_back(LevelHandler::Level_1::update);*/

			GET_COMPONENT(Transform, player).Static = false;
			GET_COMPONENT(Transform, enemy).Static = false;
			GET_COMPONENT(Transform, background).Static = false;
			
			for (uint32_t i = 0; i < 400; i++)
				GET_COMPONENT(Transform, wall[i]).Static = true;
			
			for (uint32_t i = 0; i < 10000; i++)
				GET_COMPONENT(Transform, level[i]).Static = true;
		}
		
		void Update() override  
		{
			scriptingSystem.Run(LevelManager::GetCurrentLevel());
			textureSystem.Run(renderingSystem.Get_Vertex_Buffer());
			gravitySystem.Run();
			transformSystem.Run(renderingSystem.Get_Vertex_Buffer());
			collisionSystem.Run(renderingSystem.Get_Vertex_Buffer());
			shadowSystem.Run(GET_COMPONENT(Transform, player).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
		}
	};

}

