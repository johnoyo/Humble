#pragma once

#include "Humble.h"

namespace HBL 
{
	class EnemyScript final : public IScript
	{
	public:
		EnemyScript() {}
		~EnemyScript() {}

		IEntity FPSCounter;
		IEntity player;
		IEntity enemy;
		IEntity text;

		virtual void OnCreate() override 
		{
			if (SceneManager::Get().GetCurrentLevel() == 0)
			{
				FPSCounter = (Registry::Get().FindEntityWithTag("FPSCounter"));
				text = (Registry::Get().FindEntityWithTag("Text"));
			}
			player = (Registry::Get().FindEntityWithTag("Player"));
			enemy = (Registry::Get().FindEntityWithTag("Enemy"));

			Registry::Get().GetComponent<Component::SpriteRenderer>(enemy).texture = "res/textures/player_r.png";
			//Registry::Get().GetComponent<Component::Transform>(enemy).rotation = 45.0f;
		}

		virtual void OnUpdate(float dt) override 
		{
			Registry::Get().GetComponent<Component::Transform>(enemy).position.x -= 30.0f * dt;
			//Registry::Get().GetComponent<Component::Transform>(enemy).rotation++;
			//Registry::Get().GetComponent<Component::Gravity>(enemy).Enabled = false;

			if (SceneManager::Get().GetCurrentLevel() == 0)
			{
				if (InputManager::GetKeyDown(GLFW_KEY_H))
				{
					Registry::Get().GetComponent<Component::Text>(FPSCounter).text = "fllffrrrWWWffff123LffLff";
				}
				else
				{
					Registry::Get().GetComponent<Component::Text>(FPSCounter).text = std::to_string((int)Registry::Get().GetComponent<Component::Transform>(player).position.x);
				}

				if (InputManager::GetKeyDown(GLFW_KEY_G))
				{
					Registry::Get().GetComponent<Component::Text>(FPSCounter).position.x += 30.0f * dt;
				}

				Registry::Get().GetComponent<Component::Text>(text).text = std::to_string((int)Registry::Get().GetComponent<Component::Text>(FPSCounter).position.x) + " / " + std::to_string((int)Registry::Get().GetComponent<Component::Text>(FPSCounter).position.y);
			}
		}

	};
}
