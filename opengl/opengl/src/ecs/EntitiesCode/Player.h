#pragma once
#include <iostream>
#include "../Header.h"
#include "../Utilities.h"

namespace Player {

	namespace Level_0 {

		void init(float dt) {

			//GET_COMPONENT(Transform, player).position.x = 15.0f;
			//GET_COMPONENT(Transform, player).position.y = 100.0f;
			//GET_COMPONENT(Transform, player).scale.x = 25.0f;
			//GET_COMPONENT(Transform, player).scale.y = 25.0f;
			//GET_COMPONENT(Material, player).texture = "res/textures/player_r.png";

			GET_COMPONENT(Material, sps).texture = "res/textures/super_mario_tiles.png";

			GET_COMPONENT(Material, player).subTexture.coords = { 6.0f, 1.0f };
			GET_COMPONENT(Material, player).subTexture.sprite_size = { 16.0f, 16.0f };
			GET_COMPONENT(Material, player).subTexture.path = GET_COMPONENT(Material, sps).texture;

		}

		void update(float dt) {

			// Background follow player
			GET_COMPONENT(Transform, background).position.x = GET_COMPONENT(Transform, player).position.x; /*+(-windowSystem.Get_Width() / 2.0f);*/
			GET_COMPONENT(Transform, background).position.y = GET_COMPONENT(Transform, player).position.y; /*+(-windowSystem.Get_Height() / 2.0f);*/

			// Camera follow player
			cameraSystem.Follow(player, (-windowSystem.Get_Width() / 2.0f), (-windowSystem.Get_Height() / 2.0f));

			// Player movement
			if (inputSystem.GetKeyDown(windowSystem.Get_Window(), GLFW_KEY_D, GLFW_PRESS))
				GET_COMPONENT(Transform, player).position.x += 6.0f;

			if (inputSystem.GetKeyPress(windowSystem.Get_Window(), GLFW_KEY_SPACE))
				soundSystem.PlaySound("res/audio/bleep.mp3");

			if (inputSystem.GetKeyDown(windowSystem.Get_Window(), GLFW_KEY_A, GLFW_PRESS))
				GET_COMPONENT(Transform, player).position.x -= 6.0f;

			if (inputSystem.GetKeyDown(windowSystem.Get_Window(), GLFW_KEY_S, GLFW_PRESS)) 
				GET_COMPONENT(Transform, player).position.y -= 6.0f;

			if (inputSystem.GetKeyDown(windowSystem.Get_Window(), GLFW_KEY_W, GLFW_PRESS)) 
				GET_COMPONENT(Transform, player).position.y += 6.0f;

			if (collisionSystem.CollisionBetween(player, enemy, renderingSystem.Get_Vertex_Buffer()))
				ENGINE_LOG("Player collided with enemy!!!");

			// NOTE: Maybe move this inside the gravity system??? 

			// Player movement with gravity enabled
			//if (inputSystem.GetKeyDown(windowSystem.Get_Window(), GLFW_KEY_W, GLFW_PRESS) && GET_COMPONENT(Gravity, player).isGrounded)
			//	GET_COMPONENT(Gravity, player).appliedForce = 10.0f;

			//GET_COMPONENT(Transform, player).position.y += GET_COMPONENT(Gravity, player).appliedForce;
			//std::cout << GET_COMPONENT(Gravity, player).appliedForce << "\n";
		}

	}

}

