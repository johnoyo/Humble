#pragma once

namespace HBL {

	class EnemyScript final : public IScript
	{
	public:
		EnemyScript() {}
		~EnemyScript() {}

		virtual void Init() override 
		{
			Globals::s_Registry.GetComponent<Component::SpriteRenderer>(enemy).texture = "res/textures/player_r.png";
			//Globals::s_Registry.GetComponent<Component::Transform>(enemy).rotation = 45.0f;

		}

		virtual void Update(float dt) override 
		{
			Globals::s_Registry.GetComponent<Component::Transform>(enemy).position.x -= 30.0f * dt;
			//Globals::s_Registry.GetComponent<Component::Transform>(enemy).rotation++;
			//Globals::s_Registry.GetComponent<Component::Gravity>(enemy).Enabled = false;

			if (Globals::Current_Level == 0)
			{
				if (InputManager::GetKeyDown(GLFW_KEY_H))
				{
					Globals::s_Registry.GetComponent<Component::Text>(FPSCounter).text = "fllffrrrWWWffff123LffLff";
				}
				else
				{
					Globals::s_Registry.GetComponent<Component::Text>(FPSCounter).text = std::to_string((int)Globals::s_Registry.GetComponent<Component::Transform>(player).position.x);
				}

				if (InputManager::GetKeyDown(GLFW_KEY_G))
				{
					Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).position.x += 30.0f * dt;
				}

				Globals::s_Registry.GetComponent<Component::Text>(text).text = std::to_string((int)Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).position.x) + " / " + std::to_string((int)Globals::s_Registry.GetComponent<Component::TextTransform>(FPSCounter).position.y);
			}
		}

	};

}
