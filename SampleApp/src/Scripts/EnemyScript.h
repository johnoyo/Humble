#pragma once

namespace HBL 
{
	class EnemyScript final : public IScript
	{
	public:
		EnemyScript() {}
		~EnemyScript() {}

		virtual void OnCreate() override 
		{
			Registry::Get().GetComponent<Component::SpriteRenderer>(enemy).texture = "res/textures/player_r.png";
			//Registry::Get().GetComponent<Component::Transform>(enemy).rotation = 45.0f;

		}

		virtual void OnUpdate(float dt) override 
		{
			Registry::Get().GetComponent<Component::Transform>(enemy).position.x -= 30.0f * dt;
			//Registry::Get().GetComponent<Component::Transform>(enemy).rotation++;
			//Registry::Get().GetComponent<Component::Gravity>(enemy).Enabled = false;

			if (Globals::Current_Level == 0)
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
					Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).position.x += 30.0f * dt;
				}

				Registry::Get().GetComponent<Component::Text>(text).text = std::to_string((int)Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).position.x) + " / " + std::to_string((int)Registry::Get().GetComponent<Component::TextTransform>(FPSCounter).position.y);
			}
		}

	};
}
