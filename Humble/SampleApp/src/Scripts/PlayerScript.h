#pragma once

namespace HBL {

	class PlayerScript : public ScriptFunction
	{
	public:
		PlayerScript() {}
		~PlayerScript() {}

		virtual void Init() override {

			//GET_COMPONENT(Material, player).texture = "res/textures/player_r.png";
			GET_COMPONENT(Material, player).texture = "res/textures/super_mario_tiles.png";
			GET_COMPONENT(Material, player).coords = { 6.0f, 1.0f };
			GET_COMPONENT(Material, player).sprite_size = { 16.0f, 16.0f };

			GET_COMPONENT(Animation, player).material = &GET_COMPONENT(Material, player);
			GET_COMPONENT(Animation, player).frames = 5;
			GET_COMPONENT(Animation, player).step = 0.5f;
			GET_COMPONENT(Animation, player).loop = true;
			GET_COMPONENT(Animation, player).Enabled = false;

		}

		virtual void Update() override {

			// Background follow player
			GET_COMPONENT(Transform, background).position.x = GET_COMPONENT(Transform, player).position.x;
			GET_COMPONENT(Transform, background).position.y = GET_COMPONENT(Transform, player).position.y;

			// Camera follow player
			GlobalSystems::cameraSystem.Follow(camera, player, (-GlobalSystems::windowSystem.Get_Width() / 2.0f), (-GlobalSystems::windowSystem.Get_Height() / 2.0f));

			// Player movement
			if (InputManager::GetKeyDown(GLFW_KEY_D)) {
				GET_COMPONENT(Transform, player).position.x += 6.0f;
				GET_COMPONENT(Animation, player).Enabled = true;
			}
			else
			{
				GET_COMPONENT(Animation, player).Enabled = false;
			}

			if (InputManager::GetKeyPress(GLFW_KEY_SPACE))
				SoundManager::PlaySound("res/audio/bleep.mp3");

			if (InputManager::GetKeyDown(GLFW_KEY_A)) {
				GET_COMPONENT(Transform, player).position.x -= 6.0f;
			}

			if (InputManager::GetKeyDown(GLFW_KEY_S))
				GET_COMPONENT(Transform, player).position.y -= 6.0f;

			if (InputManager::GetKeyDown(GLFW_KEY_W))
				GET_COMPONENT(Transform, player).position.y += 6.0f;

			if (GlobalSystems::collisionSystem.CollisionBetween(player, enemy))
				ENGINE_LOG("Player collided with enemy!!!");

		}

	};

}
