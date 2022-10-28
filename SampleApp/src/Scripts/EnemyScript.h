#pragma once

namespace HBL {

	class EnemyScript final : public IScript
	{
	public:
		EnemyScript() {}
		~EnemyScript() {}

		virtual void Init() override {

			//std::cout << "Calling enemy init 0 \n";
			GET_COMPONENT(Material, enemy).texture = "res/textures/player_r.png";
			//GET_COMPONENT(Transform, enemy).rotation = 45.0f;

		}

		virtual void Update(float dt) override {

			//std::cout << "Calling enemy update 0 \n";
			GET_COMPONENT(Transform, enemy).position.x -= 30.0f * dt;
			//GET_COMPONENT(Transform, enemy).rotation++;
			//GET_COMPONENT(Gravity, enemy).Enabled = false;

			if (Globals::Current_Level == 0)
			{
				if (InputManager::GetKeyDown(GLFW_KEY_H))
				{
					GET_COMPONENT(Text, FPSCounter).text = "fllffrrrWWWffff123LffLff";
				}
				else
				{
					GET_COMPONENT(Text, FPSCounter).text = std::to_string((int)GET_COMPONENT(Transform, player).position.x);
				}

				if (InputManager::GetKeyDown(GLFW_KEY_G))
				{
					GET_COMPONENT(TextTransform, FPSCounter).position.x += 30.0f * dt;
				}

				GET_COMPONENT(Text, text).text = std::to_string((int)GET_COMPONENT(TextTransform, FPSCounter).position.x) + " / " + std::to_string((int)GET_COMPONENT(TextTransform, FPSCounter).position.y);
			}
		}

	};

}
