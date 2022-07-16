#pragma once

#include "..\Core\Managers\InputManager.h"
#include "..\Core\Managers\SoundManager.h"
#include "..\Core\Utilities.h"
#include "..\Core\ScriptFunction.h"

namespace HBL {

	class PlayerScript : public ScriptFunction
	{
	public:
		PlayerScript() {}
		~PlayerScript() {}

		virtual void Init() override {

			GET_COMPONENT(Material, player).texture = "res/textures/player_r.png";
		}

		virtual void Update() override {

			// Background follow player
			GET_COMPONENT(Transform, background).position.x = GET_COMPONENT(Transform, player).position.x; /*+(-windowSystem.Get_Width() / 2.0f);*/
			GET_COMPONENT(Transform, background).position.y = GET_COMPONENT(Transform, player).position.y; /*+(-windowSystem.Get_Height() / 2.0f);*/

			// Camera follow player
			GlobalSystems::cameraSystem.Follow(camera, player, (-GlobalSystems::windowSystem.Get_Width() / 2.0f), (-GlobalSystems::windowSystem.Get_Height() / 2.0f));

			// Player movement
			if (InputManager::GetKeyDown(GlobalSystems::windowSystem.Get_Window(), GLFW_KEY_D, GLFW_PRESS)) {
				GET_COMPONENT(Material, player).texture = "res/textures/player_r.png";
				GET_COMPONENT(Transform, player).position.x += 6.0f;
			}

			if (InputManager::GetKeyPress(GlobalSystems::windowSystem.Get_Window(), GLFW_KEY_SPACE))
				SoundManager::PlaySound("res/audio/bleep.mp3");

			if (InputManager::GetKeyDown(GlobalSystems::windowSystem.Get_Window(), GLFW_KEY_A, GLFW_PRESS)) {
				GET_COMPONENT(Material, player).texture = "res/textures/player_l.png";
				GET_COMPONENT(Transform, player).position.x -= 6.0f;
			}

			if (InputManager::GetKeyDown(GlobalSystems::windowSystem.Get_Window(), GLFW_KEY_S, GLFW_PRESS))
				GET_COMPONENT(Transform, player).position.y -= 6.0f;

			if (InputManager::GetKeyDown(GlobalSystems::windowSystem.Get_Window(), GLFW_KEY_W, GLFW_PRESS))
				GET_COMPONENT(Transform, player).position.y += 6.0f;

			if (GlobalSystems::collisionSystem.CollisionBetween(player, enemy))
				ENGINE_LOG("Player collided with enemy!!!");

		}

	};

}