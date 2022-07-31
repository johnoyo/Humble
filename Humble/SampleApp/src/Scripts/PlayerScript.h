#pragma once

namespace HBL {

	class PlayerScript final : public IScript
	{
	public:
		PlayerScript() {}
		~PlayerScript() {}

		virtual void Init() override {

			//GET_COMPONENT(Material, player).texture = "res/textures/player_r.png";
			Component::Material& material = GET_COMPONENT(Material, player);
			material.texture = "res/textures/super_mario_tiles.png";
			material.coords = { 6.0f, 1.0f };
			material.sprite_size = { 16.0f, 16.0f };

			Component::Animation& animation = GET_COMPONENT(Animation, player);

			animation.animations.push_back({
				"WalkAnim",
				&material,
				{ 6.0f, 1.0f },
				0.0,
				0.5,
				5,
				5,
				true,
				false
			});

			animation.animations.push_back({
				"JumpAnim",
				&material,
				{ 5.0f, 2.0f },
				0.0,
				0.5,
				5,
				5,
				true,
				false
			});

			animation.animations[0].Enabled = false;
			animation.animations[1].Enabled = false;
		}

		virtual void Update() override {

			Component::Transform& transform_p = GET_COMPONENT(Transform, player);
			Component::Transform& transform_bg = GET_COMPONENT(Transform, background);
			Component::Animation& animation_p = GET_COMPONENT(Animation, player);

			// Background follow player
			transform_bg.position.x = transform_p.position.x;
			transform_bg.position.y = transform_p.position.y;

			// Camera follow player
			GlobalSystems::cameraSystem.Follow(camera, player, (-GlobalSystems::windowSystem.Get_Width() / 2.0f), (-GlobalSystems::windowSystem.Get_Height() / 2.0f));
			
			// Player movement
			if (InputManager::GetKeyDown(GLFW_KEY_D)) {
				transform_p.position.x += 6.0f;
				animation_p.animations[0].Enabled = true;
			}
			else
			{
				animation_p.animations[0].Enabled = false;
			}
			if (InputManager::GetKeyRelease(GLFW_KEY_D))
				GlobalSystems::animationSystem.ResetAnimation(animation_p, 0, 5);

			if (InputManager::GetKeyPress(GLFW_KEY_SPACE))
				SoundManager::PlaySound("res/audio/bleep.mp3");

			if (InputManager::GetKeyDown(GLFW_KEY_A)) {
				transform_p.position.x -= 6.0f;
			}

			if (InputManager::GetKeyDown(GLFW_KEY_S))
				transform_p.position.y -= 6.0f;

			if (InputManager::GetKeyDown(GLFW_KEY_W))
			{
				transform_p.position.y += 6.0f;
				animation_p.animations[1].Enabled = true;
			}
			else
			{
				animation_p.animations[1].Enabled = false;
			}
			if (InputManager::GetKeyRelease(GLFW_KEY_W))
				GlobalSystems::animationSystem.ResetAnimation(animation_p, 1, 5);

			if (GlobalSystems::collisionSystem.CollisionBetween(player, enemy))
				ENGINE_LOG("Player collided with enemy!!!");

		}

	};

}
