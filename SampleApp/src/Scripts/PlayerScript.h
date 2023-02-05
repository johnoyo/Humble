#pragma once

#include "Humble.h"

namespace HBL 
{
	class PlayerScript final : public IScript
	{
	public:
		PlayerScript() {}
		~PlayerScript() {}

		IEntity player;
		IEntity enemy;
		IEntity background;
		IEntity camera;
		IEntity clipCamera;
		IEntity tile;
		IEntity pinkTile;

		virtual void OnCreate() override 
		{
			player = (Registry::Get().FindEntityWithTag("Player"));
			enemy = (Registry::Get().FindEntityWithTag("Enemy"));
			background = (Registry::Get().FindEntityWithTag("Background"));
			camera = (Registry::Get().FindEntityWithTag("Camera"));
			tile = Registry::Get().FindEntityWithTag("Tile");
			pinkTile = Registry::Get().FindEntityWithTag("PinkTile");

			if (SceneManager::Get().GetCurrentLevel() == 0)
				clipCamera = (Registry::Get().FindEntityWithTag("ClipCamera"));

			Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(player);
			sprite.texture = "res/textures/super_mario_tiles.png";
			sprite.coords = { 6.0f, 1.0f };
			sprite.spriteSize = { 16.0f, 16.0f };

			Component::Animation& animation = Registry::Get().GetComponent<Component::Animation>(player);

			animation.animations.push_back({
				"WalkAnim",
				&sprite,
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
				&sprite,
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

		virtual void OnUpdate(float dt) override 
		{
			Component::Transform& transform_p = Registry::Get().GetComponent<Component::Transform>(player);
			Component::Transform& transform_bg = Registry::Get().GetComponent<Component::Transform>(background);
			Component::Transform& transform_cam = Registry::Get().GetComponent<Component::Transform>(camera);
			Component::Animation& animation_p = Registry::Get().GetComponent<Component::Animation>(player);

			if (SceneManager::Get().GetCurrentLevel() == 0)
			{
				if (InputManager::GetKeyDown(GLFW_KEY_C))
				{
					Registry::Get().GetComponent<Component::Camera>(camera).primary = false;
					Registry::Get().GetComponent<Component::Camera>(clipCamera).primary = true;
				}
				else 
				{
					Registry::Get().GetComponent<Component::Camera>(camera).primary = true;
					Registry::Get().GetComponent<Component::Camera>(clipCamera).primary = false;
				}
			}

			// Background follow player
			transform_bg.position.x = transform_p.position.x;
			transform_bg.position.y = transform_p.position.y;

			// Camera follow player
			transform_cam.position.x = transform_p.position.x + (-Systems::Window.GetWidth() / 2.0f);
			transform_cam.position.y = transform_p.position.y + (-Systems::Window.GetHeight() / 2.0f);
			
			// Add shadow component at runtime.
			if (InputManager::GetKeyPress(GLFW_KEY_P))
			{
				Registry::Get().GetComponent<Component::SpriteRenderer>(pinkTile).Enabled = false;

				Registry::Get().AddComponent<Component::Shadow>(tile);

				VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

				Component::Shadow& shadow = Registry::Get().GetComponent<Component::Shadow>(tile);
				shadow.source = &player;

				if (shadow.Enabled && shadow.source != nullptr)
				{
					Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(tile);

					sprite.texture = "-";
					sprite.color = shadow.color;

					shadow.parentBufferIndex = Registry::Get().GetComponent<Component::Transform>(tile).bufferIndex;
					shadow.bufferIndex = buffer.GetSize();
				}

				if (shadow.Enabled && shadow.source != nullptr)
				{
					glm::vec3& O = Registry::Get().GetComponent<Component::Transform>(*shadow.source).position;

					// Find all shadow points
					for (int j = 0; j < 4; j++)
					{
						glm::vec2& E = buffer.GetBuffer()[shadow.parentBufferIndex + j].position;

						float rdx, rdy;
						rdx = E.x - O.x;
						rdy = E.y - O.y;

						float base_ang = atan2f(rdy, rdx);

						rdx = shadow.shadowDistance * cosf(base_ang);
						rdy = shadow.shadowDistance * sinf(base_ang);

						shadow.points.push_back({ rdx, rdy });
					}

					// Set shadow quad positions
					Renderer::Get().RegisterQuad(0, 
													buffer.GetBuffer()[shadow.parentBufferIndex + 3].position,
													shadow.points[3], shadow.points[0],
													buffer.GetBuffer()[shadow.parentBufferIndex + 0].position,
													shadow.color);

					Renderer::Get().RegisterQuad(0,
													buffer.GetBuffer()[shadow.parentBufferIndex + 0].position,
													shadow.points[0], shadow.points[1],
													buffer.GetBuffer()[shadow.parentBufferIndex + 1].position,
													shadow.color);

					Renderer::Get().RegisterQuad(0, 
													buffer.GetBuffer()[shadow.parentBufferIndex + 1].position,
													shadow.points[1],
													shadow.points[2],
													buffer.GetBuffer()[shadow.parentBufferIndex + 2].position, shadow.color);
				}
			}

			// Remove shadow component at runtime.
			if (InputManager::GetKeyRelease(GLFW_KEY_P))
			{
				Registry::Get().GetComponent<Component::Shadow>(tile).Enabled = false;
				Registry::Get().RemoveComponent<Component::Shadow>(tile);
				Registry::Get().GetComponent<Component::SpriteRenderer>(pinkTile).Enabled = true;
			}

			// Player movement
			if (InputManager::GetKeyDown(GLFW_KEY_D)) 
			{
				transform_p.position.x += 210.0f * dt;
				Systems::Animation.PlayAnimation(animation_p, 0);
			}
			else
			{
				Systems::Animation.StopAnimation(animation_p, 0);
			}
			if (InputManager::GetKeyRelease(GLFW_KEY_D))
				Systems::Animation.ResetAnimation(animation_p, 0, 5);

			if (InputManager::GetKeyPress(GLFW_KEY_SPACE))
				SoundManager::Play("res/audio/bleep.mp3", false, false);

			if (InputManager::GetKeyDown(GLFW_KEY_A)) {
				transform_p.position.x -= 210.0f * dt;
			}

			if (InputManager::GetKeyDown(GLFW_KEY_S) && SceneManager::Get().GetCurrentLevel() == 1)
				transform_p.position.y -= 210.0f * dt;

			if (InputManager::GetKeyDown(GLFW_KEY_W))
			{
				if (SceneManager::Get().GetCurrentLevel() == 1)
					transform_p.position.y += 210.0f * dt;
				else
					if (Registry::Get().GetComponent<Component::Gravity>(player).isGrounded)
						Registry::Get().GetComponent<Component::Gravity>(player).appliedForce = 180.f;

				Systems::Animation.PlayAnimation(animation_p, 1);
			}
			else
			{
				Systems::Animation.StopAnimation(animation_p, 1);
			}
			if (InputManager::GetKeyRelease(GLFW_KEY_W))
				Systems::Animation.ResetAnimation(animation_p, 1, 5);

			if (Systems::Collision.CollisionBetween(player, enemy))
				ENGINE_LOG("Player collided with enemy!!!");
		}

	};
}
