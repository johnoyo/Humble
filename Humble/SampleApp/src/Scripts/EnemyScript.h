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

		virtual void Update() override {

			//std::cout << "Calling enemy update 0 \n";
			//GET_COMPONENT(Transform, enemy).position.x--;
			//GET_COMPONENT(Transform, enemy).rotation++;
			//GET_COMPONENT(Gravity, enemy).Enabled = false;

		}

	};

}
